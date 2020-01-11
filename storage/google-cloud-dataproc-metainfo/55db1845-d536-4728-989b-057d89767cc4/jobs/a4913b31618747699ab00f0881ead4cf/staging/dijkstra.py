#!/usr/bin/env python

import timeit
import pyspark
import sys

inputUri='gs://dataproc-ed3c3d29-fb10-47bb-aca7-dcc358c68973-us-central1/input/map.txt'
outputUri='gs://dataproc-ed3c3d29-fb10-47bb-aca7-dcc358c68973-us-central1/output'

sc = pyspark.SparkContext()
grid = []

def parseRDD(line):
    parsedLine = list(line)
    return parsedLine

lines = sc.textFile(inputUri)
words = lines.flatMap(parseRDD)
wordsMap = lines.map(parseRDD).collect()

for i in range(len(wordsMap)):
    row = []
    for j in range(len(wordsMap[i])):
        row.append(0)
    grid.append(row)
    
sharedGrid = sc.broadcast(grid)

def dijkstra(word):
    localGrid = sharedGrid.value
    for i in range(len(localGrid)):
        for j in range(len(localGrid[i])):
            localGrid[i][j] = len(localGrid[i])*i+j
    return (word, localGrid[len(localGrid)-1][len(localGrid)-1])

start_time = timeit.default_timer()
words.map(dijkstra)
elapsed = timeit.default_timer() - start_time
sharedElapsed = sc.broadcast(elapsed)
rddGrid = words.map(lambda word: (word, sharedElapsed.value))
wordCounts = words.map(lambda word: (word, 1)).reduceByKey(lambda count1, count2: count1 + count2)
wordCounts.saveAsTextFile(outputUri)
rddGrid.saveAsTextFile(outputUri+'/debug')