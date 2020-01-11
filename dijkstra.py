#!/usr/bin/env python

import timeit
import pyspark
import sys, os

import math
import heapq

from grid import Grid

inputUri='gs://dataproc-ed3c3d29-fb10-47bb-aca7-dcc358c68973-us-central1/input/map.txt'
outputUri='gs://dataproc-ed3c3d29-fb10-47bb-aca7-dcc358c68973-us-central1/output'

def parseRDD(line):
  parsedLine = list(line)
  return parsedLine

sc = pyspark.SparkContext()

grid = []

lines = sc.textFile(inputUri, 6)
words = lines.flatMap(parseRDD)
wordsMap = lines.map(parseRDD).collect()

grid = Grid([len(wordsMap), len(wordsMap[0])])
for  i in xrange (len(wordsMap)):
  for  j in xrange (len(wordsMap[0])):
  	if wordsMap[i][j] == 'T':
  		grid.mGrid[i][j].is_obs= True

sharedGrid = sc.broadcast(grid)

def buildCpd(cell):
  localGrid = sharedGrid.value
  for  i in xrange (len(localGrid.mGrid)):
    for  j in xrange (len(localGrid.mGrid[i])):
    	localGrid.mGrid[i][j].dir = -1
    	localGrid.mGrid[i][j].neighbours = {}
    	localGrid.mGrid[i][j].visited = False
    	localGrid.mGrid[i][j].weight = 1000
    	localGrid.mGrid[i][j].father = {}
    	localGrid.mGrid[i][j].inHeap = False

  start_time = timeit.default_timer()
  localGrid.dijkstra(localGrid.mGrid[cell.pos[0]][cell.pos[1]])
  
  '''for  i in xrange (len(localGrid.mGrid)):
    for  j in xrange (len(localGrid.mGrid[i])):
    	val_grid  =  val_grid + str(localGrid.mGrid[i][j].dir) + ', '
    val_grid  =  val_grid + "\n"'''
  re = localGrid.compress(localGrid.mGrid[cell.pos[0]][cell.pos[1]])
  id = cell.pos[0]*localGrid.dims[1]+cell.pos[1]
  elapsed = timeit.default_timer() - start_time
  info_rect = " {"
  for i in range(len(re)):
  	info_rect = info_rect+"-["+str(re[i].start.pos[0])+","+str(re[i].start.pos[1])+")("+str(re[i].end.pos[0])+","+str(re[i].end.pos[1])+"]"
  info_rect = info_rect+"} "
  return (cell.pos[0], cell.pos[1], id,len(re),info_rect, elapsed)

rdda = sc.parallelize(grid.mGrid, 21)
rddb = rdda.flatMap(lambda row: row)
rddc = rddb.map(buildCpd)
rddc.saveAsTextFile(outputUri+'/debug')
