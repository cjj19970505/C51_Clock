#define TASKSELECTOR_MODE_CLOCK 0
#define TASKSELECTOR_MODE_DATE 1
#define TASKSELECTOR_MODE_STOPWATCH 2
#define TASKSELECTOR_CURRENT_MODE ((const char)taskSelector_Mode)

extern char taskSelector_Mode;

void TaskSelector_LooperUpdate(LOOPER *looper);
