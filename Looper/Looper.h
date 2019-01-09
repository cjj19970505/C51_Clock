#define LOOPER struct Looper
#define LOOPER_MAX_LOAD 5	//ADDRESS SPACE OVERFLOW when it gets to 10, maybe a linklist is needed
struct Looper
{
	int inited;
	int deltaTime;	//delta time in millsecond
	unsigned int taskCount;
	void (*(updateFunctions[LOOPER_MAX_LOAD]))(LOOPER *looper);
};
void Looper_Init(LOOPER *looper);
void Looper_AddTask(LOOPER *looper, void (*updateFunc)(LOOPER*));
void Looper_Update(LOOPER *looper, int deltaTimeInMS);

//void Looper_AddTask(LOOPER *looper, void (*updateFunc)(LOOPER*));