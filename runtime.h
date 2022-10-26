#ifndef __RUNTIME_H__
#define __RUNTIME_H__

/**
 * Initializes the simulation
 */
void init_simulation(int seed, int num_jobs);

/**
 *  Returns the number of jobs in the simulation.
 */
int get_num_jobs();

/** 
 * Returns the current simulation time starting from 0.
 */
int get_current_time();

/**
 * Runs the specified job for the specified amount of time. 
 * If the job has finished its operation then -1 is returned. 
 * If job fully used the allocated time then timeout is 
 * returned. If the job is blocked on an IO operation then a number
 * between 0 and timeout-1 is returned indicating how long the
 * job has actually run before the IO operation. This operation 
 * has an overhead of one time unit.
 */
int schedule_job(int job_id, int timeout, int start);

#endif //__RUNTIME_H__
