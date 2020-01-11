# Local  Coordination  Diagrams  for  Collision  Avoidance  in  Multi-Robot Path  Planning
Abstract: Autonomous robots are intended to help humans performing a lot of different tasks in a more safer and efficient way. Some of those tasks must be solved by a group of autonomous robots. Solutions including multiple robots have to solve group problems like communication and coordination; also, common problems of autonomous robots like the widely studied problem of Path Planning must be rethought. In this case, finding a collision-free path is not enough because each robot also has to avoid collisions with other robots (by coordinating their movements). In this scenario, the path planning problem turns into the multi-robot motion planning (MRMP) problem. There are two approaches for solving the MRMP problem: coupled and decoupled. This work is focused on the decoupled approach because it has the potential to solve MRMP not only in a centralized way but, also in a concurrent or distributed way. In this sense, a new parallelizable algorithm, called Local Coordination Diagrams - LCD, is presented in this paper. Experimental results show that our approach can be applied efficiently to a large number of robots.

## Experiments on real maps

Berlin Scenario with 100 paths to be coordinated. Black cells represent obstacles. Paths to be coordinated are represented in gray cells. Additionally, the start cell is marked with a random coloured square and the last cell is marked with a triangle colored with the same color of the path where it belongs.


## To Compile
Please always generate the executables following the examples *myprog.out, myprog.exe*.

#### To compile de grid example example.cpp:
```bat
g++ -std=c++11 example.cpp grid.hpp
```

## Issues
Every issue has a branch assigned in their descripion

#### To start working on an issue:
Open your terminal in the directory where this repo was clonned and do:
```bat
git checkout develop
git pull origin develop
```
If the branch of the issue does not exist:
```bat
git branch branch_of_the_issue
git checkout branch_of_the_issue
```
Else if the branch already exists:
```bat
git checkout branch_of_the_issue
git pull origin branch_of_the_issue
```

#### To commit and push:
Every issue has an id, example: '#6' so add it to the description of the commits to keep track of your work:
```bat
git commit -m "#id Description"
git push origin branch_of_the_issue
```

#### To finish with an issue:
```
1. Go to Pull requests
2. New pull request
3. base: develop <- compare: branch_of_the_issue
4. Create pull request
5. Assign reviewers
```
