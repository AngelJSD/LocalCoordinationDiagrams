#!/usr/bin/env python

import pyspark
import sys

inputUri='gs://dataproc-ed3c3d29-fb10-47bb-aca7-dcc358c68973-us-central1/input/map.txt'
outputUri='gs://dataproc-ed3c3d29-fb10-47bb-aca7-dcc358c68973-us-central1/output'

sc = pyspark.SparkContext()
grid = []

def parseRDD(line):
    parsedLine = list(line)
    #row = []
    #for i in range(len(parsedLine)):
    #    row.append(0)
    #sharedGrid.append(row)
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

rddGrid = words.map(lambda word: (word, 1, sharedGrid.value[4][4]))
wordCounts = words.map(lambda word: (word, 1)).reduceByKey(lambda count1, count2: count1 + count2)
wordCounts.saveAsTextFile(outputUri)
rddGrid.saveAsTextFile(outputUri+'/debug')