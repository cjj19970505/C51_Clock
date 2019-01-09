#define BIN_GET_BIT(bin, index) ((1<<index) & (bin))
#define BIN_SET_BIT(bin, index, value) ((value) ? ((bin) | 1<<(index)) : ( (~(1<<(index))) & (bin) ))