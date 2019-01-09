#define SEGSCREEN_SHIFT_INTERVAL_MS 1

void SegScreen_LooperUpdate(LOOPER *looper);
void SegScreen_SetLed(int index, char ledMask);
//void SegScreen_Print_Int(int value);
//void SegScreen_Print_Bin(int value);
void SegScreen_Print_String(char *str);