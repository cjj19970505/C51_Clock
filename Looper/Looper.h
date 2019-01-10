#define LOOPER struct Looper
#define LOOPER_MAX_LOAD 6	//ADDRESS SPACE OVERFLOW when it gets to 10, maybe a linklist is needed
struct Looper
{
	char inited;
	char deltaTime;	//delta time in millsecond
};
void Looper_Init(LOOPER *looper);
void Looper_Update(LOOPER *looper, int deltaTimeInMS);