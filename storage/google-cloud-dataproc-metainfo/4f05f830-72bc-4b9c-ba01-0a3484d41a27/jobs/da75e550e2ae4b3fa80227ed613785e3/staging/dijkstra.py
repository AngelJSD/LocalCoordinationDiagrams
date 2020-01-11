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

lines = sc.textFile(inputUri, 18)
words = lines.flatMap(parseRDD)
wordsMap = lines.map(parseRDD).collect()

grid = Grid([len(wordsMap), len(wordsMap[0])])

#for i in range(len(wordsMap)):
#  row = []
#  for j in range(len(wordsMap[i])):
#    row.append(0)
#  grid.append(row)
    
sharedGrid = sc.broadcast(grid)

def buildCpd(cell):
  return (cell.pos[0], cell.pos[1])

sc.parallelize(grid.mGrid).map(buildCpd).saveAsTextFile(outputUri+'/debug')

def dijkstra(word):
  localGrid = sharedGrid.value
  start_time = timeit.default_timer()
  for i in range(len(localGrid)):
    for j in range(len(localGrid[i])):
      localGrid[i][j].weight = len(localGrid[i])*i+j
  elapsed = timeit.default_timer() - start_time
  return (word, localGrid[len(localGrid) - 1][len(localGrid) - 1].weight, elapsed)

# rddDijkstra = words.map(dijkstra)

#rddGrid = rddDijkstra.reduceByKey(lambda count1, count2: count1 + count2)
#wordCounts = words.map(lambda word: (word, 1)).reduceByKey(lambda count1, count2: count1 + count2)
#wordCounts.saveAsTextFile(outputUri)
# rddDijkstra.saveAsTextFile(outputUri+'/debug')
