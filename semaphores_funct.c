#include <stdio.h>
#include <stdlib.h>
#include "definitions.h"

//copy the process to the semaphore
void copy(semaphore* s, process* p) {
        s->pr.pid = p->pid;
		s->pr.pr = p->pr;
		s->pr.tol = p->tol;
        s->pr.to_cs = p->to_cs;
        s->pr.old_pr = p->old_pr;
}

//copy the process (s->pr) from the semaphore to p
void copy_i(semaphore* s, process* p) {
        p->pid=s->pr.pid;
       	p->pr=s->pr.pr;
        p->tol=s->pr.tol;
        p->to_cs=s->pr.to_cs;
        p->old_pr=s->pr.old_pr;
}


