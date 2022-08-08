#define time_c 0.1

typedef struct {
	int pid;
	int pr;
	int tol;
    int to_cs;
    int old_pr;
} process;

typedef struct {
	int state;
	process pr;
	int Waitting_t;
} semaphore;


typedef struct QueueNodeTag {
	process pr;
	struct QueueNodeTag *Link;
} QueueNode;

typedef struct {
	QueueNode *Front;
	QueueNode *Rear;
	int Waitting_t;
} Queue;


