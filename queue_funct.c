
#include <stdio.h>
#include <stdlib.h>
#include "definitions.h"


void InitializeQueue(Queue *Q){
    
	Q->Front=NULL;
    Q->Rear=NULL;
	Q->Waitting_t=0;
}

int Empty(Queue *Q) {
   
	return(Q->Front==NULL);

}

void Insert(process R, Queue *Q) {
   
	QueueNode *Temp;
   	Temp=(QueueNode *)malloc(sizeof(QueueNode));
   	if (Temp==NULL){
    	printf("System storage is exhausted");
   	} 
	else {
      	Temp->pr=R;
      	Temp->Link=NULL;
      	if (Q->Rear==NULL){
         	Q->Front=Temp;
         	Q->Rear=Temp;
      	} 	
		else {
         	Q->Rear->Link=Temp;
         	Q->Rear=Temp;
      	}
	}
}


void Remove(Queue *Q, process *F) {
   
	QueueNode *Temp;
   	if (Q->Front==NULL){
    	printf("attempt to remove item from an empty queue");
   	} 
	else {
    	*F=Q->Front->pr;
      	Temp=Q->Front;
      	Q->Front=Temp->Link;
      	free(Temp);
      	if (Q->Front==NULL) Q->Rear=NULL;
	}
}
    


