#define TIME struct Time
#define TIME_GET_SECOND(time) ((time).second)
#define TIME_GET_DECISECOND(time) ((time).decisecond)
#define TIME_GET_MINUTE(time) ((time).minute)
#define TIME_GET_HOUR(time) ((time).hour)
#define TIME_SET_SECOND(time, value) ((time).second = (value))
#define TIME_SET_DECISECOND(time, value) ((time).decisecond = (value))
#define TIME_SET_MINUTE(time, value) ((time).minute = (value))
#define TIME_SET_HOUR(time, value) ((time).hour = (value))


struct Time
{
	char decisecond;
	char second;
	char minute;
	char hour;
};

void Time_ToString(TIME *time, char hyphen, char hasDeciSecond, char *str);
void Time_Add_1_Decisecond(TIME *time);