Assignment 7
============

**Due: October 24, 2022 by 11:59 PM US Central Time**. Push to GitHub to submit the assignment.

In this assignment you are going to write two simple schedulers that might be used in an OS
based on the "CPU Scheduling" and "Multi-level Feedback" chapters from the [Operating Systems](http://pages.cs.wisc.edu/~remzi/OSTEP/) book.

A simple simulator and an example FIFO scheduler are already written for you so that you can focus on writing your scheduler. In the simulator we create multiple jobs at initialization time. Each job can have several CPU intensive parts that are separated by IO activity. For example, the line

```
Creating job 0 with parts 4 15
```

means that job 0 will have two CPU intensive parts in which the first lasts for 4 time units, the second for 15 time
units, and after each we have a short amount of IO. The job of the scheduler is to call the `schedule_job` method with the `job_id` and `timeout` parameters and the simulator will perform this execution by maintaining the simulation time, and returning from `schedule_job` early if the job wants to execute an IO operation. Please see the documentation in `runtime.h` and the code in `runtime.c`.

The execution of the FIFO scheduler is demonstrated below (you might get slightly different numbers):

```
larvellejones@policeacademy:~/cs3281/assignment-7/build$ ./scheduler fifo
Creating job 0 with parts 4 15
Creating job 1 with parts 4 18 6
job 0 started at 0
job 0 run for 4 at 5
job 0 run for 9 at 15
job 0 run for 6 at 22
job 0 finished at 23
job 1 started at 23
job 1 run for 4 at 28
job 1 run for 9 at 38
job 1 run for 9 at 48
job 1 run for 0 at 49
job 1 run for 6 at 56
job 1 finished at 57
Average turnaround time was 40.000000
Average response time was 11.500000
```

The FIFO scheduler schedules the jobs one after the other and uses 9 time units as a timeout parameter. Note, that even though job 0 is run for 9 units the elapsed time is 10 since the scheduler has an overhead. The presence of an IO operation is indicated by a return value smaller than the timeout value.

## TASKS

* (40 points) Write a Round-Robin scheduler in `scheduler_rr`. Peroperly calculate the turnaround and response times. Make the time slice parameter configurable in `main` using the `-t` flag.

* (60 points) Write a Multi-Level Feedback Queue scheduler in `scheduler_mlfq`. Use 4 queue levels and fixed arrays to hold the queues. Implement only Rules 1, 2, 3, 4a and 4b.
Properly calculate and report the turnaround and response times. 

* (10 extra points) Implement Rules 4 and 5 from the book with configurable parameters for the MLFQ scheduler.

