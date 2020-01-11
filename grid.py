import sys, os
import math
import heapq

from cell import Cell
from rectangle import Rectangle

class Grid:
  def __init__(self, dims):
    self.dims = dims
    self.mGrid =[[Cell([row,column]) for column in xrange(dims[0])]  
                      for row in xrange(dims[1])] 
    self.cpd = dict()   

  def dijkstra(self,start):
    print "init Dijkstra"
    self.initial_directions(start)
    self.mGrid[start.pos[0]][start.pos[1]].weight = 0
    heap = []
    heapq.heappush(heap,(start.weight,start))
    self.mGrid[start.pos[0]][start.pos[1]].inHeap = True
    while len(heap):
      current = heap[0][1]
      heapq.heappop(heap)
      self.mGrid[current.pos[0]][current.pos[1]].visited = True
      n = self.get_neightbours(current)
      for i in xrange( len(n)):
        if self.mGrid[n[i].pos[0]][n[i].pos[1]].visited == False:
          newd = current.weight + current.Edistance(n[i])
          if newd <= n[i].weight:
            self.mGrid[n[i].pos[0]][n[i].pos[1]].weight = newd
            if current != start:
              self.mGrid[n[i].pos[0]][n[i].pos[1]].dir = current.dir
            if self.mGrid[n[i].pos[0]][n[i].pos[1]].inHeap == False:
              heapq.heappush(heap,(newd,n[i]))
              self.mGrid[n[i].pos[0]][n[i].pos[1]].inHeap = True


  def initial_rectangles(self, start):
    rects = []  
    if start.pos[1]+1 < self.dims[1]:
      rects.append(Rectangle(self.mGrid[start.pos[0]][start.pos[1]+1], self.mGrid[start.pos[0]][self.dims[1]-1])) # right
      if start.pos[0]+1 < self.dims[0]:    
        rects.append(Rectangle(self.mGrid[start.pos[0]+1][start.pos[1]+1], self.mGrid[self.dims[0]-1][self.dims[1]-1])) # right-down  
      if start.pos[0]-1 >= 0:
        rects.append(Rectangle(self.mGrid[0][start.pos[1]+1], self.mGrid[start.pos[0]-1][self.dims[1]-1])) # right-up
    if start.pos[0]+1 < self.dims[0]:
      rects.append(Rectangle(self.mGrid[start.pos[0]+1][start.pos[1]], self.mGrid[self.dims[0]-1][start.pos[1]])) # down
      if start.pos[1]-1 >= 0:
        rects.append(Rectangle(self.mGrid[start.pos[0]+1][0], self.mGrid[self.dims[0]-1][start.pos[1]-1])) # left-down  
    if start.pos[1]-1 >= 0:
      rects.append(Rectangle(self.mGrid[start.pos[0]][0], self.mGrid[start.pos[0]][start.pos[1]-1])) # left
    if start.pos[0]-1 >= 0:    
      rects.append(Rectangle(self.mGrid[0][start.pos[1]], self.mGrid[start.pos[0]-1][start.pos[1]])) # up
      if start.pos[1]-1 >= 0:
       rects.append(Rectangle(self.mGrid[0][0], self.mGrid[start.pos[0]-1][start.pos[1]-1])) # left-up
    return rects

  def process_rect(self, rect):
    homogeneus = True
    dir = -2
    flag_dir_assigned = False
    tiles = 0
    
    for i in xrange(rect.start.pos[0], rect.end.pos[0]+1):
      for j in xrange(rect.start.pos[1], rect.end.pos[1]+1):
        current = self.mGrid[i][j]    
        if not current.isObstacle():
          if current.dir != dir:
            if not flag_dir_assigned:
              dir = current.dir
              flag_dir_assigned = True
            else :
              dir = -1
              homogeneus = False
              return tiles, homogeneus, dir
          tiles = tiles+1
    return tiles, homogeneus, dir
  
  def left_homogenize(self, rect):
    homogeneus = True
    split = rect.start.pos[1]
    flag = False
    while homogeneus and split+1 <= rect.end.pos[1]:
      flag = True
      split = split +1
      n_split = self.mGrid[rect.end.pos[0]][split]
      p_rect = Rectangle(rect.start,n_split)
      t,homogeneus,dir = self.process_rect(p_rect)
    if flag == True:
      r1 = Rectangle(rect.start,self.mGrid[rect.end.pos[0]][split-1])
    else:
      ##print "useFlag"
      r1 = Rectangle(rect.start,self.mGrid[rect.end.pos[0]][split])
    t,h,d = self.process_rect(r1)
    ##print "topL:",
    ##r1.print_comp()
    if h == True:
      r1.set_features(t,h,d)
      return r1, 1, split
    else:
      return -1, -1, -1

  def right_homogenize(self, rect):
    homogeneus = True
    split = rect.end.pos[1]
    flag = False
    while homogeneus and split-1 >= rect.start.pos[1]:
      flag = True
      split = split -1
      n_split = self.mGrid[rect.start.pos[0]][split]
      p_rect = Rectangle(n_split,rect.end)
      t,homogeneus,dir = self.process_rect(p_rect)
    if flag == True:
      r1 = Rectangle(self.mGrid[rect.start.pos[0]][split+1],rect.end)
    else:
      ##print "useFlag"
      r1 = Rectangle(self.mGrid[rect.start.pos[0]][split],rect.end)
    t,h,d = self.process_rect(r1)
    ##print "topR:",
    ##r1.print_comp()
    if h == True:
      r1.set_features(t,h,d)
      return r1, 2, split 
    else:
      return -1, -1, -1

  def top_homogenize(self,rect):
    homogeneus = True
    split = rect.start.pos[0]
    flag = False
    while homogeneus and split+1 <= rect.end.pos[0]:
      flag = True
      split = split +1
      n_split = self.mGrid[split][rect.end.pos[1]]
      p_rect = Rectangle(rect.start,n_split)
      t,homogeneus,dir = self.process_rect(p_rect)
    if flag == True:
      r1 = Rectangle(rect.start,self.mGrid[split-1][rect.end.pos[1]])
    else:
      ##print "useFlag"
      r1 = Rectangle(rect.start,self.mGrid[split][rect.end.pos[1]])
    t,h,d = self.process_rect(r1)
    ##print "topH:",
    ##r1.print_comp()
    if h == True:
      r1.set_features(t,h,d)
      return r1, 3, split
    else:
      return -1, -1, -1

  def bottom_homogenize(self, rect):
    homogeneus = True
    split = rect.end.pos[0]
    flag = False
    while homogeneus and split-1 >= rect.start.pos[0]:
      flag = True
      split = split -1
      n_split = self.mGrid[split][rect.start.pos[1]]
      p_rect = Rectangle(n_split,rect.end)
      t,homogeneus,dir = self.process_rect(p_rect)
    if flag == True:
      r1 = Rectangle(self.mGrid[split+1][rect.start.pos[1]],rect.end)
    else:
      ##print "useFlag"
      r1 = Rectangle(self.mGrid[split][rect.start.pos[1]],rect.end)
    t,h,d = self.process_rect(r1)
    ##print "topB:",
    ##r1.print_comp()
    if h == True:
      r1.set_features(t,h,d)
      return r1, 4, split
    else:
      return -1, -1, -1

  def compRect(self, rect, opt, split):
    if opt == 1: #left ok
      return Rectangle(self.mGrid[rect.start.pos[0]][split],rect.end)
    if opt == 2: #right ok
      return Rectangle(rect.start, self.mGrid[rect.end.pos[0]][split])
    if opt == 3: #top ok
      return Rectangle(self.mGrid[split][rect.start.pos[1]],rect.end)
    if opt == 4: #bottom
      return Rectangle(rect.start, self.mGrid[split][rect.end.pos[1]])

  def homogenize(self, rect,result):
    ##print "\nrect before homogenize",
    ##rect.print_comp()
    t,h,d = self.process_rect(rect)
    if h == True :
      result.append(rect)
      return
    
    maxTiles = 0
    optSplit = 0
    split = 0
    hRect = ""
    r1, opt1, split1 = self.left_homogenize(rect)
    if r1 != -1:
      ##print "left_h:", r1.tiles, "opt:",opt1
      if r1.tiles > maxTiles:
        maxTiles = r1.tiles
        optSplit = opt1
        split = split1
        hRect = r1
    r2, opt2, split2 = self.top_homogenize(rect)
    if r2 != -1:
      ##print "top_h:", r2.tiles, "opt:",opt2
      if r2.tiles > maxTiles:
        maxTiles = r2.tiles
        optSplit = opt2
        split = split2
        hRect = r2
    
    r3, opt3, split3 = self.right_homogenize(rect)
    if r3 != -1:
      ##print "right_h:", r3.tiles, "opt:",opt3
      if r3.tiles > maxTiles:
        maxTiles = r3.tiles
        optSplit = opt3
        split = split3
        hRect = r3

    r4, opt4, split4 = self.bottom_homogenize(rect)
    if r4 != -1:
      ##print "bottom_h:", r4.tiles, "opt:",opt4
      if r4.tiles > maxTiles:
        maxTiles = r4.tiles
        optSplit = opt4
        split = split4   
        hRect = r4

    ##print "maxTile:", maxTiles, "optSplit", optSplit
    if maxTiles == 0:
      ##print "default_top_split"
      split = rect.start.pos[0]
      d1 = Rectangle(rect.start,self.mGrid[split][rect.end.pos[1]])
      t,h,d = self.process_rect(d1)
      d1.set_features(t,h,d)
      d2 = Rectangle(self.mGrid[split+1][rect.start.pos[1]],rect.end)
      t,h,d = self.process_rect(d2)
      d2.set_features(t,h,d)
      ##print "hrect_d1",
      ##d1.print_comp()
      ##print "hrect_d2",
      ##d2.print_comp()
      self.homogenize(d1, result)
      self.homogenize(d2, result)

    else:
      compRect = self.compRect(rect, optSplit, split)
      t,h,d = self.process_rect(compRect)
      ##print "hrect",
      ##hRect.print_comp()
      #print "comprect",
      ##compRect.print_comp()
      compRect.set_features(t,h,d)
      result.append(hRect)
      self.homogenize(compRect, result)

  def printGrid(self):
    for  i in xrange (len(self.mGrid)):
      for  j in xrange (len(self.mGrid[i])):
        print self.mGrid[i][j].dir, '\t',
      print "\n",
 
  def get_neightbours(self,start):
    n = []   
    if start.pos[1]+1 < self.dims[1]: # right
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
  
  def id_cell(self, start):
    id = start.pos[0]*self.dims[1]+start.pos[1]
    return id

  def compress(self, start):  
    ##print "compress for",
    ##start.printCell2d()
    frects = []
    irects = self.initial_rectangles(start)
    ##print "initial rects:",len(irects),
    for r in xrange(len(irects)):
      t,h,d = self.process_rect(irects[r])
      if h == True:
        irects[r].set_features(t,h,d)
        frects.append(irects[r])
        #irects[r].print_comp()
      else:  
        self.homogenize(irects[r],frects)
    id = self.id_cell(start)
    ##print "save to cpd id:", id
    self.cpd[id] = frects    
    return frects
