#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "interface.h"
#include <time.h>

int simulation(int num_sem ,double l1, double l2,double l3,double k ,int max_pr) {
	

	int i, toa,arr_counter;
	semaphore sem[num_sem];
    Queue Sem_q[num_sem], Pr_q[7] ;
	float p;

//initialization of num_sem semaphores , array of queues for the semaphores
	
	for (i=0 ; i<num_sem ; i++) {
		sem[i].state=1;
		sem[i].pr.pid = -1;
        sem[i].pr.pr = -1;
        sem[i].pr.tol = -1;
        sem[i].pr.to_cs = -1;
		sem[i].Waitting_t=0;
        sem[i].pr.old_pr=-1;
        InitializeQueue(&Sem_q[i]);
	}

//calculating the time between arrivals
 
	srand((unsigned int)time(NULL));
	float a = 1.0;
    p=(float)rand()/(float)(RAND_MAX) * a;
    p=log(p);
    p=p*(-1);
    p=((float)1/l1)*p;
    toa=p/time_c;
	if(toa!=(p/time_c)) 
		toa++;
	printf("This is the estimated time between arrivals: %d  time slots\n" ,toa);
      


//initialize the array of queues (1-7 priority)

	for(i=0 ; i<7 ; i++) 
		InitializeQueue(&Pr_q[i]);


//Start of main loop(1 iteration->1 time slot)

	arr_counter=0;
    int pid=0,ex_flag=0;

	while(pid<max_pr || ex_flag==0) {
		
//calculate the waitting time of the processes in the priority queues
        printf("Time slot: %d\n\n",arr_counter+1);
        for(i=0 ; i<7 ; i++) {
			if(Empty(&Pr_q[i])==0)
				Pr_q[i].Waitting_t++;
		}
        
		int xi[7];
		for(i=0 ;i<7; i++)
			xi[i]=0;               //in every time slot xi=0(xi shows if there is at least one process of a specific priority at the queue of a semaphore)
		for(i=0 ; i<num_sem ; i++) {
         	if(Empty(&Sem_q[i])==0) {
				int wait=0,max=sem[i].pr.pr;
				Queue Q = Sem_q[i];
                
				while(Q.Front!=NULL) {
					if(Q.Front->pr.pr < sem[i].pr.pr && sem[i].pr.tol>0){
						wait=1;					//if wait=1 then the process which is executing the cs has lower priority from at least one process in the queue of the semaphore
					}
					if(Q.Front->pr.pr == 1 && xi[0]==0) {
						xi[0]=1;                                        //check every process in every semaphores queue
						Pr_q[0].Waitting_t++;
					}
					if(Q.Front->pr.pr == 2 && xi[1]==0) { 
                        Pr_q[1].Waitting_t++;
						xi[1]=1;
					}
					if(Q.Front->pr.pr == 3 && xi[2]==0) { 
                        Pr_q[2].Waitting_t++;
						xi[2]=1;
					}
					if(Q.Front->pr.pr == 4 && xi[3]==0) { 
                        Pr_q[3].Waitting_t++;
						xi[3]=1;
					}
					if(Q.Front->pr.pr == 5 && xi[4]==0) { 
                        Pr_q[4].Waitting_t++;
						xi[4]=1;
					}
					if(Q.Front->pr.pr == 6 && xi[5]==0) { 
                        Pr_q[5].Waitting_t++;
						xi[5]=1;
					}
					if(Q.Front->pr.pr == 7 && xi[6]==0) { 
                        Pr_q[6].Waitting_t++;
						xi[6]=1;
					}
                    if(Q.Front->pr.pr < max)
                        max =Q.Front->pr.pr;                //priority invertion-find the max priority for each queue
                    
					Q.Front=Q.Front->Link;                  //move on to the next element of the queue
				}
                int flag3=0;
                if(sem[i].pr.pr>max ) {
                    flag3=1;
                    printf("The process with pid: %d was updated from priority %d to %d\n\n", sem[i].pr.pid, sem[i].pr.pr, max);
                    if(sem[i].pr.old_pr==0)
                        sem[i].pr.old_pr=sem[i].pr.pr;
                    sem[i].pr.pr=max;
                }
				if(wait==1 &&flag3==0)
					sem[i].Waitting_t++;
                
			}	
      
			if(sem[i].state == 0) {
                sem[i].pr.to_cs--;
				if(sem[i].pr.to_cs== -1 && sem[i].pr.tol>0) {                       //if the cs' processing is completed but the process has still tof->remove the process should be removed from the semaphore and be inserted to the suitable priority queue
					printf("the process %d will be removed from semaphore!\n\n",sem[i].pr.pid);
                    if(sem[i].pr.old_pr!=0) {
                        sem[i].pr.pr=sem[i].pr.old_pr;
                        sem[i].pr.old_pr=0;
                        
                    }
					process temp_pr;
					copy_i(&sem[i], &temp_pr);
					if (temp_pr.pr==1)
                  		Insert(temp_pr,&Pr_q[0]);
            		if (temp_pr.pr==2)
                		Insert(temp_pr,&Pr_q[1]);
            		if (temp_pr.pr==3)
                		Insert(temp_pr,&Pr_q[2]);
            		if (temp_pr.pr==4)
                		Insert(temp_pr,&Pr_q[3]);
            		if (temp_pr.pr==5)
                		Insert(temp_pr,&Pr_q[4]);
            		if (temp_pr.pr==6)
                		Insert(temp_pr,&Pr_q[5]);
            		if (temp_pr.pr==7)
                		Insert(temp_pr,&Pr_q[6]);
					sem[i].state=1;
				}
                else if(sem[i].pr.to_cs== -1 && sem[i].pr.tol==0) {   //if the cs' processing is completed and the process has run out of life time->just remove the process from the sem
                    printf("the process %d will be removed from semaphore!\n",sem[i].pr.pid);
                    sem[i].state=1;
                }
				else if(sem[i].pr.to_cs!=-1)                          //if the cs>0 just decrease it
					sem[i].pr.tol--;
					
				if(sem[i].state==1) {                                 //if the sem is now available->insert the first process in the queue!
					if(Empty(&Sem_q[i])==0) {
						process temp_pr;
						Remove(&Sem_q[i], &temp_pr);
						copy(&sem[i], &temp_pr);
						sem[i].state=0;
                        printf("The process with pid: %d has just been inserted in the semaphore: %d\n", temp_pr.pid, i+1);
					}
				}
			}
		}




//check for arrivals

		if(arr_counter%toa == 0 && pid<max_pr) {
                                                                    //there is a new arrival assign a priority and tol
											
			srand((int)1050*arr_counter*pid);
			process temp_pr;
            temp_pr.pr= rand()%7 +1;
			temp_pr.pid=++pid;
			
            srand((unsigned int)time(NULL));
		    float a = 1.0;
    		p=(float)rand()/(float)(RAND_MAX) * a;
    		p=log(p);
    		p=p*(-1);
   			p=((float)1/l2)*p;
			int tol=p/time_c;
    		if(tol!=(p/time_c))
        		tol++;
			temp_pr.tol=tol;
    		
            temp_pr.old_pr=0;
            
			temp_pr.to_cs=-1;

            printf("At time slot: %d we had a new arrival with pid: %d and priority: %d\n", arr_counter+1, temp_pr.pid, temp_pr.pr);
			printf("This is the estimated time of life of the process with pid %d :: %d time slots\n\n" ,pid ,temp_pr.tol);
			
									//insert the arrival at the suitable queue
            if (temp_pr.pr==1)
				Insert(temp_pr,&Pr_q[0]);
			if (temp_pr.pr==2)
                Insert(temp_pr,&Pr_q[1]);
			if (temp_pr.pr==3)
                Insert(temp_pr,&Pr_q[2]);
			if (temp_pr.pr==4)
                Insert(temp_pr,&Pr_q[3]);
			if (temp_pr.pr==5)
                Insert(temp_pr,&Pr_q[4]);
			if (temp_pr.pr==6)
                Insert(temp_pr,&Pr_q[5]);
			if (temp_pr.pr==7)
                Insert(temp_pr,&Pr_q[6]);
		}

//starting calculate propabilities for each queue's first elememt
	    i=0;
        while(i<7 ) {
			int flag = 0;
			if(Empty(&Pr_q[i])!=1 ) {
				
				
                                                        //there is at least one process with the highest priority
				srand((unsigned int)6585324*arr_counter);
    			float a = 1.0;
    			p=(float)rand()/(float)(RAND_MAX) * a;
                    if (p<k) {
                    printf("The propability of the first process(pid: %d) of the Q%d is %f\n", Pr_q[i].Front->pr.pid, i+1,  p);
                    flag=1;
					srand((int)3551347*arr_counter*round(p));
					int sem_d = rand()% num_sem ;
                	printf("The process with pid %d will attempt a down operation in semaphore %d\n", Pr_q[i].Front->pr.pid, sem_d+1);
					if(sem[sem_d].state == 1) {

															//down op was succeded
						printf("The process with pid: %d made a successful down operation at the semaphore: %d\n", Pr_q[i].Front->pr.pid, sem_d+1);
						sem[sem_d].state=0;
						int x=0;
						while (x==0) {
															//calculate cs time (cs time MUST be < tol)
							srand((unsigned int)time(NULL));
		            		float a = 1.0;
							sleep(2);
        		    		p=(float)rand()/(float)(RAND_MAX) * a;
                            p=log(p);
            				p=p*(-1);
           					p=((float)1/l3)*p;
            				int to_cs=p/time_c;
            				if(to_cs!=(p/time_c))
                				to_cs++;
							if(to_cs<=Pr_q[i].Front->pr.tol) {
                        		Pr_q[i].Front->pr.to_cs = to_cs;
								x=1;
								printf("The cs time: %d is acceptable for process: %d !\n\n", to_cs ,Pr_q[i].Front->pr.pid);
							}
				
						}
															//pop the process from the queue and insert it to the semaphore
						copy(&sem[sem_d], &(Pr_q[i].Front->pr));
						process temp;
						Remove(&Pr_q[i], &temp);
 
					}
														//an other process is already in the semaphore
					else {
						printf("The process with pid: %d DID NOT succed at the down operation in semaphore: %d and its in the queue\n", Pr_q[i].Front->pr.pid, sem_d+1);
						int x=0;
                        while (x==0) {
                                                            //calculate cs time (cs time MUST be < tol)
                            srand((unsigned int)time(NULL));
                            float a = 1.0;
							sleep(2);
                            p=(float)rand()/(float)(RAND_MAX) * a;
							p=log(p);
                            p=p*(-1);
                            p=((float)1/l3)*p;
                            int to_cs=p/time_c;
                            if(to_cs!=(p/time_c))
                                to_cs++;
                            if(to_cs<=Pr_q[i].Front->pr.tol) {
                                Pr_q[i].Front->pr.to_cs = to_cs;
                                x=1;
                                printf("The cs time: %d is acceptable for process: %d !\n\n", to_cs ,Pr_q[i].Front->pr.pid);
                            }
			
						}
                        process temp;
                    	Remove(&Pr_q[i], &temp);
						Insert(temp, &Sem_q[sem_d]);
					}
					
				}
				if(Empty(&Pr_q[i])==1 || flag==0)
                	i++;
			
			}
			else 
				i++;

		}
        

		arr_counter++;
		int flag1=0;
        for (i=0 ; i<7 ; i++) {
			if(Empty(&Pr_q[i])!=1)
				flag1=1;
		}
		int flag2=0;
		if(flag1==0) {
			for(i=0 ; i<num_sem ; i++) {
				if(Empty(&Sem_q[i])!= 1 || sem[i].state == 0) 
					flag2=1;
			}
			if (flag2==0) 
				ex_flag=1;
		}
	

	}
	for (i=0 ; i<7 ; i++)
		printf("The waitting time for priority queue %d is %d !\n", i+1, Pr_q[i].Waitting_t); 
    
 	for(i=0 ; i<num_sem ; i++)
		printf("The waiting time for processes with higher priority in semaphore %d is: %d\n", i+1, sem[i].Waitting_t);	

	return 0;
}
