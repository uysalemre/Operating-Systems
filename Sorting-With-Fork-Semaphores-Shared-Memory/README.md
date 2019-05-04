## What is Shared Memory ? 
   - When you are using processes and when you need to reach a memory area with more than one process you need to use Shared Memory. 
   - You can easily think it like pointer operations.
     - If you want to use you need take memory.
     - When your work is done you need to give it back.
 
## What is Semaphores ?
   - When you need to manage processes the solution is Semaphores.
   - You can use Semaphores in 2 types. 
     - Mutual Exculution
       - You can use Mutual Exclusion Semaphores between child processes.
     - Synchronization
       - You can use Synchronization Semaphores for managing working orders between parent and child processes.
   
## What is included in this Project ?  
  ### FORK
      - It is used for creating child processes. 
      - 1.Child = 1. Array Sorting  
      - 2.Child = 2. Array Sorting  
      - 3.Child = Merge and Sort

  ### Shared Memory
      - It is used for keeping and reaching array that we used.
  
  ### Semaphores
      - It is used for Mutex operation and Synchronization between processes.
  
  ### How to run ?
      gcc project.c -o project
      ./project
