# What is Fork ?
  - Unix Operating Systems use a special function named Fork for creating new processes.
  - For example; when you open your Linux computer all processes start with using fork() from root process.
  - The easy way to understand it is drawing process tree.
  - Example : In parent fork called -> child created | Again fork called -> a new child created | From second child fork called -> Now there is a child attached to second child
  - Hierarchy is like : 
    - Root
      - 1 st child 
      - 2 nd child
        - Child of 2nd child
        
## Extra Knowledge ? 
   - You can't manage when will a process end or you can't order them to work. So you must sleep it or other ways :
   - Other ways : You can use Semaphores.
   
## How to see processes in Linux ? 
   - you can use command : top
   - another way is : ps -aux 
   
## Run 

    gcc project.c -o project
    
    ./project
