# CloudPathPlanning
Here is the first attempt of a CloudPathPlanning method

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
