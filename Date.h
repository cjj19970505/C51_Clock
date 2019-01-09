#define DATE struct Date
#define DATE_SET_YEAR(date, value) ((date).year = (value) - 2000)
#define DATE_SET_MONTH(date, value) ((date).month = (value))
#define DATE_SET_DAY(date, value) ((date).day = (value))
#define DATE_GET_YEAR(date) ((date).year + 2000)
#define DATE_GET_MONTH(date) ((date).month)
#define DATE_GET_DAY(date) ((date).day)
struct Date
{
	char year;
	char month;
	char day;
};