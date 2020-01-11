#!/usr/bin/env python

import pyspark
import sys

inputUri='gs://dataproc-ed3c3d29-fb10-47bb-aca7-dcc358c68973-us-central1/input/map.txt'
outputUri='gs://dataproc-ed3c3d29-fb10-47bb-aca7-dcc358c68973-us-central1/output'

grid = []

def parseRDD(line):
    parsedLine = list(line)
    row = []
    for i in range(len(parsedLine)):
        row.append(0)
    grid.append(row)
    return parsedLine

sc = pyspark.SparkContext()
lines = sc.textFile(inputUri)
words = lines.flatMap(parseRDD)
rddGrid = words.map(lambda word: (word, 1, grid[2][2]))
wordCounts = words.map(lambda word: (word, 1)).reduceByKey(lambda count1, count2: count1 + count2)
wordCounts.saveAsTextFile(outputUri)
rddGrid.saveAsTextFile(outputUri+'/debug')