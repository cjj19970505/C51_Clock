#define INPUT_CHECKINPUT_INTERVAL_MS 50
#define INPUT_TOKEYCODE(row, column) ((1 << (row-1)) | (1 << (column-1+4)))
void Input_LooperUpdate(LOOPER *looper);
char Input_GetLatestKeyCode();