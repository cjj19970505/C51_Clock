#define TIME struct Time
#define TIME_GET_SECOND(time) ((time).second)
#define TIME_GET_MINUTE(time) ((time).minute)
#define TIME_GET_HOUR(time) ((time).hour)
#define TIME_SET_SECOND(time, value) ((time).second = (value))
#define TIME_SET_MINUTE(time, value) ((time).minute = (value))
#define TIME_SET_HOUR(time, value) ((time).hour = (value))

struct Time
{
	int millsecond;
	int second;
	int minute;
	int hour;
};

void Time_ToString(TIME *time, char *str);