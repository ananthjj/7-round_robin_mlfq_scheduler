#include "runtime.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern job_t jobs;

void scheduler_fifo()
{
    int num_jobs = get_num_jobs();
    int arrival = get_current_time();

    float total_turnaround = 0.0f, total_response = 0.0f;
    for (int i = 0; i < num_jobs; i++) {
        int firstrun = get_current_time();
        printf("job %d started at %d\n", i, firstrun);
        total_response += firstrun - arrival;

        int r;
        while ((r = schedule_job(i, 9, 0)) >= 0)
            printf("job %d run for %d at %d\n", i, r, get_current_time());

        int completion = get_current_time();
        printf("job %d finished at %d\n", i, completion);

        total_turnaround += completion - arrival;
    }

    printf("Average turnaround time was %f\n", total_turnaround / num_jobs);
    printf("Average response time was %f\n", total_response / num_jobs);
}

void scheduler_rr()
{
  int quantum = 4;
  int num_jobs = get_num_jobs();
  int arrival = get_current_time();
  int completion;
  float total_turnaround = 0.0f, total_response = 0.0f;
  char* status = malloc(sizeof(char) * num_jobs);
  
  for (int b = 0; b < num_jobs; b++){
    status[b] = 'b'; //beginning
  }
  
  int flag = 0;
  int start = 0;
  do{
    for(int i = 0; i < num_jobs; i++){
      if (status[i] == 'b'){
	int firstrun = get_current_time();
	printf("job %d started at %d\n", i, firstrun);
	status[i] = 'p'; //processing
	total_response += firstrun - arrival;
      }
      int compVal = schedule_job(i,quantum,start);
      if (compVal == -1){
	completion = get_current_time();
	status[i] = 'f'; //finished
	total_turnaround += completion - arrival;
      }
      else{
	
      }
    }

    flag = 0;
    for (int c = 0; c < num_jobs; c++){
      if(status[c] != 'f')
	flag = 1;
    }
	
  } while(flag==0);

  free(status);
  status = NULL;
  
  printf("Average turnaround time was %f\n", total_turnaround / num_jobs);
  printf("Average response time was %f\n", total_response / num_jobs);
}

void scheduler_mlfq()
{
  
}

void help()
{
    printf("Usage: scheduler [-s seed] [-n jobs] fifo|rr|mlfq\n");
    exit(1);
}

int main(int argc, char* argv[])
{
    int seed = 4;
    int jobs = 2;

    char c;
    while ((c = getopt(argc, argv, "s:n:h")) != -1) {
        switch (c) {
        case 's':
            seed = atoi(optarg);
            break;

        case 'n':
            jobs = atoi(optarg);
            break;

        default:
            help();
        }
    }

    init_simulation(seed, jobs);

    if (argv[optind] == NULL || strcmp(argv[optind], "fifo") == 0)
        scheduler_fifo();
    else if (strcmp(argv[optind], "rr") == 0)
        scheduler_rr();
    else if (strcmp(argv[optind], "mlfq") == 0)
        scheduler_mlfq();
    else
        help();

    return 0;
}
