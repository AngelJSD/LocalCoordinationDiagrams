#!/usr/bin/env python

import timeit
import pyspark
import sys

import math
import heapq

class Cell:
  def __init__(self, pos):
    self.pos = pos
    self.is_obs = False
    self.neighbours = {}
    self.visited = False  
    self.weight = sys.maxint
    self.father = {}
    self.dir = -1
  
  def printCell2d(self):
    print "(",self.pos[0],",",self.pos[1],")"
#   for i in range(len(self.pos)):
#     print self.pos[i]
  def printCell3d(self):
    print "(",self.pos[0],",",self.pos[1],",",self.pos[2],")"

  def setAsObstacle(self):
    self.is_obs = True
  
  def setAsVisited(self):
    self.visited = True
  
  def setNeighbours(self, neighbours):
    self.neighbours = neighbours

  def isObstacle(self):
    return self.is_obs
  
  def Edistance(self, other):
#    self.printCell2d()
#    other.printCell2d()
    return round(math.sqrt((float(self.pos[0])-float(other.pos[0]))**2 + (float(self.pos[1])-float(other.pos[1]))**2),1)
  

class Grid:
  def __init__(self, dims):
    self.dims = dims
    self.mGrid=[[Cell([row,column]) for column in range(dims[0])]  
                      for row in range(dims[1])] 
    
  def printGrid(self):
    for  i in range (len(self.mGrid)):
      for  j in range (len(self.mGrid[i])):
        print self.mGrid[i][j].dir,
      print "\n",
  
  def get_neightbours(self,start):
    n = []   
    if start.pos[1]+1 < self.dims[1]: # derecha
      var1 = self.mGrid[start.pos[0]][start.pos[1]+1]      
      if not var1.isObstacle():
        n.append(var1)
      if start.pos[0]+1 < self.dims[0]: #abajo y derecha
        var2 = self.mGrid[start.pos[0]+1][start.pos[1]+1]
        if not var2.isObstacle():
          n.append(var2)
      if start.pos[0]-1 >= 0: #arriba y derecha
        var3 = self.mGrid[start.pos[0]-1][start.pos[1]+1]
        if not var3.isObstacle():
          n.append(var3)
    if start.pos[0]+1 < self.dims[0]: #abajo
      var4 = self.mGrid[start.pos[0]+1][start.pos[1]]
      if not var4.isObstacle():
        n.append(var4)
      if start.pos[1]-1 >= 0: #abajo y izquierda
        var5 = self.mGrid[start.pos[0]+1][start.pos[1]-1]
        if not var5.isObstacle():
          n.append(var5)
    if start.pos[0]-1 >= 0: #arriba
      var6 = self.mGrid[start.pos[0]-1][start.pos[1]]
      if not var6.isObstacle():
        n.append(var6)
      if start.pos[1]-1 >= 0: #arriba y izquierda
        var7 = self.mGrid[start.pos[0]-1][start.pos[1]-1]
        if not var7.isObstacle():
          n.append(var7)
    if start.pos[1]-1 >= 0: # izquierda
          var8 = self.mGrid[start.pos[0]][start.pos[1]-1]
          if not var8.isObstacle():
            n.append(var8)
#    print "neightbours of "+str(start.pos[0])+","+str(start.pos[1])
#    for i in range(len(n)):
#      print n[i].pos
    return n

  def initial_directions(self,start):
    if start.pos[1]+1 < self.dims[1]: # derecha
      var1 = self.mGrid[start.pos[0]][start.pos[1]+1]      
      if not var1.isObstacle():
        var1.dir = 0
      if start.pos[0]+1 < self.dims[0]: #abajo y derecha
        var2 = self.mGrid[start.pos[0]+1][start.pos[1]+1]
        if not var2.isObstacle():
          var2.dir = 1
      if start.pos[0]-1 >= 0: #arriba y derecha
        var3 = self.mGrid[start.pos[0]-1][start.pos[1]+1]
        if not var3.isObstacle():
          var3.dir = 7
    if start.pos[0]+1 < self.dims[0]: #abajo
      var4 = self.mGrid[start.pos[0]+1][start.pos[1]]
      if not var4.isObstacle():      
        var4.dir = 2
      if start.pos[1]-1 >= 0: #abajo y izquierda
        var5 = self.mGrid[start.pos[0]+1][start.pos[1]-1]
        if not var5.isObstacle():        
          var5.dir = 3
    if start.pos[0]-1 >= 0: #arriba
      var6 = self.mGrid[start.pos[0]-1][start.pos[1]]
      if not var6.isObstacle():
        var6.dir = 6
      if start.pos[1]-1 >= 0: #arriba y izquierda
        var7 = self.mGrid[start.pos[0]-1][start.pos[1]-1]
        if not var7.isObstacle():        
          var7.dir = 5
    if start.pos[1]-1 >= 0: # izquierda
      var8 = self.mGrid[start.pos[0]][start.pos[1]-1]
      if not var8.isObstacle():
        var8.dir = 4

  def dijkstra(self,start):
    print "init Dijkstra"
    self.initial_directions(start)
    start.weight = 0
    self.printGrid()
    heap = []
    heapq.heappush(heap,(start.weight,start))
    while len(heap):
      current = heap[0][1]
#      print "current",current
      heapq.heappop(heap)
      current.setAsVisited()
      n = self.get_neightbours(current)
#      print "type(n)",type(n)
      for i in range( len(n)):
        if n[i].visited == False:
          newd = current.weight + current.Edistance(n[i])
#          print n[i].pos," newd ",newd, "n[i].weight ", n[i].weight
          if newd <= n[i].weight:
            n[i].weight = newd
            if current != start:
              n[i].dir = current.dir
            heapq.heappush(heap,(newd,n[i]))

inputUri='gs://dataproc-ed3c3d29-fb10-47bb-aca7-dcc358c68973-us-central1/input/map.txt'
outputUri='gs://dataproc-ed3c3d29-fb10-47bb-aca7-dcc358c68973-us-central1/output'

def parseRDD(line):
  parsedLine = list(line)
  return parsedLine

def main():
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

  def dijkstra(word):
    localGrid = sharedGrid.value.mGrid
    start_time = timeit.default_timer()
    for i in range(len(localGrid)):
      for j in range(len(localGrid[i])):
        localGrid[i][j].weight = len(localGrid[i])*i+j
    elapsed = timeit.default_timer() - start_time
    return (word, localGrid[len(localGrid) - 1][len(localGrid) - 1].weight, elapsed)

  rddDijkstra = words.map(dijkstra)

  #rddGrid = rddDijkstra.reduceByKey(lambda count1, count2: count1 + count2)
  #wordCounts = words.map(lambda word: (word, 1)).reduceByKey(lambda count1, count2: count1 + count2)
  #wordCounts.saveAsTextFile(outputUri)
  rddDijkstra.saveAsTextFile(outputUri+'/debug')

if __name__== "__main__":
  main()