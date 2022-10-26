#include "runtime.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_NUM_JOBS 10
#define MAX_NUM_PARTS 3
#define MAX_PART_TIME 20

int num_jobs;
int current_time;
int timeSlice;

struct job_t {
    int num_parts;
    int parts[MAX_NUM_PARTS];
    int index; // which part we are executing
    int step; // amount of work within this part
};

struct job_t jobs[MAX_NUM_JOBS];

void init_simulation(int seed, int _num_jobs)
{
  current_time = 0;

    num_jobs = _num_jobs;
    if (num_jobs < 0 || num_jobs > MAX_NUM_JOBS)
        num_jobs = MAX_NUM_JOBS;

    srand(seed);
    for (int i = 0; i < num_jobs; i++) {
        printf("Creating job %d with parts", i);
        jobs[i].num_parts = (rand() % MAX_NUM_PARTS) + 1;
        for (int s = 0; s < jobs[i].num_parts; s++) {
            jobs[i].parts[s] = (rand() % MAX_PART_TIME) + 1;
            printf(" %d", jobs[i].parts[s]);
        }
        printf("\n");
        jobs[i].index = 0;
        jobs[i].step = 0;
    }
}

int get_num_jobs()
{
    return num_jobs;
}

int get_current_time()
{
    return current_time;
}

int schedule_job(int job_id, int timeout)
{
    if (job_id < 0 || job_id >= num_jobs || timeout <= 0) {
        printf("invaid parameters\n");
        exit(1);
    }

    current_time += 1;
    if (jobs[job_id].index >= jobs[job_id].num_parts)
        return -1;

    int work;
    for (work = 0; work < timeout; work++) {
        if (jobs[job_id].step >= jobs[job_id].parts[jobs[job_id].index]) {
            jobs[job_id].step = 0;
            jobs[job_id].index += 1;
            break;
        } else
            jobs[job_id].step += 1;
    }

    current_time += work;
    return work;
}
