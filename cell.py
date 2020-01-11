import sys
import math
import heapq

class Cell:
  def __init__(self, pos):
    self.pos = pos
    self.is_obs = False
    self.neighbours = {}
    self.visited = False  
    self.weight = 1000
    self.father = {}
    self.dir = -1
    self.inHeap = False
  
  def printCell2d(self):
    print "(",self.pos[0],",",self.pos[1],")"

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
    return round(math.sqrt((float(self.pos[0])-float(other.pos[0]))**2 + (float(self.pos[1])-float(other.pos[1]))**2),1)
  