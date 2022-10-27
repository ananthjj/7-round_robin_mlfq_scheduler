#include "runtime.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int tS = 4;
int timeS = 10;

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
        while ((r = schedule_job(i, 9)) >= 0)
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
  int num_jobs = get_num_jobs();
  int arrival = get_current_time();
  int completion;
  float total_turnaround = 0.0f, total_response = 0.0f;
  char* status = malloc(sizeof(char) * num_jobs);
  
  for (int b = 0; b < num_jobs; b++){
    status[b] = 'b'; //beginning
  }
  
  int flag = 0;
  do{
    for(int i = 0; i < num_jobs; i++){
      if (status[i] == 'b'){
	int firstrun = get_current_time();
	printf("job %d started at %d\n", i, firstrun);
	status[i] = 'p'; //processing
	total_response += firstrun - arrival;
      }
      if (status[i] != 'f'){
	int compVal = schedule_job(i,tS);
	if (compVal == -1){
	  completion = get_current_time();
	  printf("job %d finished at %d\n", i, completion);
	  status[i] = 'f'; //finished
	  total_turnaround += completion - arrival;
	}
	else{
	  printf("job %d run for %d at %d\n", i, compVal, get_current_time());
	}
      }
      
      flag = 0;
      for (int c = 0; c < num_jobs; c++){
	if(status[c] != 'f')
	  flag = 1;
      }
	
    }
  }while(flag==1);

  free(status);
  status = NULL;
  
  printf("Average turnaround time was %f\n", total_turnaround / num_jobs);
  printf("Average response time was %f\n", total_response / num_jobs);
}

#define QSIZE 100

int adjust_queue(int* Q, int size){
  int index = 0;
  for (int i = 0; i<size; i++){
    if(Q[i] != -1){
      Q[index++] = Q[i];
    }
  }
  return index;
}

float processQueue(int* HQ, int i, float total_turnaround, int* LQ , int* LQsize, int arrival, char* status){
  if (HQ[i] != -1){
    int compVal = schedule_job(HQ[i], tS);
    if (compVal == -1){
      int completion = get_current_time();
      printf("job %d finished at %d\n", HQ[i], completion);
      status[HQ[i]] = 'f'; //finished
      total_turnaround += completion - arrival;
    }
    else{
      LQ[(*LQsize)++] = HQ[i];
      HQ[i] = -1;
    }
  }
  return total_turnaround;
}

int checkStatus(int* Q, int size, char* status){
  for (int i = 0; i<size; i++){
    if (status[Q[i]] != 'f')
	return 0;
  }
  return 1;
}

void moveJobs(int* HQ, int hqsize, int* LQ, int lqsize){
  for (int i = 0; i<lqsize; i++){
    HQ[hqsize++] = LQ[i];
  }
}

void moveAllJobs(int* Q1, int q1size, int* Q2, int q2size, int* Q3, int q3size, int* Q4, int q4size){
  moveJobs(Q4,q4size,Q3,q3size);
  moveJobs(Q4,q4size,Q2,q2size);
  moveJobs(Q4,q4size,Q1,q1size);
}

void scheduler_mlfq()
{
  int Q1[QSIZE], Q2[QSIZE], Q3[QSIZE], Q4[QSIZE];
  int q1size = 0, q2size = 0, q3size = 0, q4size = 0;
  int num_jobs = get_num_jobs();
  int arrival = get_current_time();
  float total_turnaround = 0.0f, total_response = 0.0f;
  char* status = malloc(sizeof(char) * num_jobs);

  int flag = 0;
  for (int b = 0; b < num_jobs; b++){
    Q4[b] = b;
    status[b] = 'b'; //beginning
    q4size++;
  }
  
  do{
    for (int i = 0; i < q4size; i++){
      if (status[Q4[i]] == 'b'){
	int firstrun = get_current_time();
        printf("job %d started at %d\n", i, firstrun);
	status[Q4[i]] = 'p'; //processing
        total_response += firstrun - arrival;
      }
      total_turnaround = processQueue(Q4, i, total_turnaround, Q3, &q3size, arrival, status);
      q4size = adjust_queue(Q4, q4size);
    }

    for (int j = 0; j < q3size; j++){
      if (checkStatus(Q4, q4size, status) == 1){
	total_turnaround = processQueue(Q3, j, total_turnaround, Q2, &q2size, arrival, status);
	q3size = adjust_queue(Q3, q3size);
      }
      else
	break;
    }
    for (int k = 0; k < q2size; k++){
      if (checkStatus(Q3, q3size, status) == 1 && checkStatus(Q4, q4size, status) == 1){
	total_turnaround = processQueue(Q2, k, total_turnaround, Q1, &q1size, arrival, status);
	q2size = adjust_queue(Q2, q2size);
      }
      else
	break;
    }
    for (int l = 0; l<q1size; l++){
      if(checkStatus(Q2, q2size, status) == 1 && checkStatus(Q3, q3size, status) == 1 && checkStatus(Q4, q4size, status) == 1){
	int compVal = schedule_job(Q1[l], tS);
	if (compVal == -1){
	  int completion = get_current_time();
	  printf("job %d finished at %d\n", Q1[l], completion);
	  status[Q1[l]] = 'f'; //finished
	  total_turnaround += completion - arrival;
	}
	q1size = adjust_queue(Q1, q1size);
      }
      else
	break;
    }


    flag = 0;
    for (int c = 0; c < num_jobs; c++){
	if(status[c] != 'f')
	  flag = 1;
      }
    
  } while(flag == 1);
  free(status);
  status = NULL;
  
  printf("Average turnaround time was %f\n", total_turnaround / num_jobs);
  printf("Average response time was %f\n", total_response / num_jobs);
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
    while ((c = getopt(argc, argv, "s:n:t:S:h")) != -1) {
        switch (c) {
        case 's':
            seed = atoi(optarg);
            break;

        case 'n':
            jobs = atoi(optarg);
            break;

	case 't':
	    tS = atoi(optarg);
	    break;

	case 'S':
	  timeS = atoi(optarg);
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
