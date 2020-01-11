class Rectangle:
  def __init__(self, start, end):
    self.start = start
    self.end = end
    self.tiles = 0
    self.homogeneus = False
    self.dir = -1
  
  def print_rdims(self):
    print " s:",
    self.start.printCell2d()
    print " e:",
    self.end.printCell2d()
  
  def set_features(self, tiles, homogeneus, dir):
    self.tiles = tiles
    self.homogeneus = homogeneus
    self.dir = dir

  def print_comp(self):
    self.print_rdims()
    print "tiles:",self.tiles, "h:",self.homogeneus, "dir:",self.dir
