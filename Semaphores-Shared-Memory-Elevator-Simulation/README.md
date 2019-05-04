## ELEVATOR-SIMULATION-SEMAPHORES-SHAREDMEMORY

![ELEVATOR ALGORITHM](https://people.ece.cornell.edu/land/courses/ece4760/FinalProjects/s2007/aoc6_dah64/aoc6_dah64/states.png)

HOW TO RUN?

      gcc project.c -o project

      ./project NumOfFloors CapasityOfElevator MaxPersonInBuilding MaxSimulationTimeForAPerson

This is a detailed simulation of an elevator which uses Semaphores, Processes and Shared Memory with C language. 

There is N floors in a building and there is an elevator with M capasity.

Every person in the building is a child process.(Every person is randomly distributed to floors).

Elevator is parent process.

A random number defines the time for spending in a floor. After waiting person calls the elevator.

### PERSON PSEUDO CODE

  Start

  1-Calls the elevator.

  2-Waits for the elevator.

  3-Gets in the elevator.

  4-Push the button for target floor.

  5-Waits for reaching the destination.

  6-Gets out from the elevator.

  End
 
### ELEVATOR PSEUDO CODE
  
  Start( 1 st floor)
  
  1-Wait for a call
  
  2-Determine the uppest or lowest floor to go
  
  3-Go to destination floor and if anybody waits in that way or anybody wants to get out in that way do that operation
  
  4-Reach the destination floor wait for person to get out
  
  End
