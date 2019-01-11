#define BIN_GET_BIT(bin, index) (((1<<index) & (bin))==0 ? 0 : 1)
#define BIN_SET_BIT(bin, index, value) ((value) ? ((bin) = ((bin) | 1<<(index))) : ((bin) = ((~(1<<(index))) & (bin) )))