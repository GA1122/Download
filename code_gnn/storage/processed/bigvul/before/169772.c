exsltDateDayName (const xmlChar *dateTime)
{
    static const xmlChar dayNames[8][10] = {
        { 0 },
	{ 'S', 'u', 'n', 'd', 'a', 'y', 0 },
	{ 'M', 'o', 'n', 'd', 'a', 'y', 0 },
	{ 'T', 'u', 'e', 's', 'd', 'a', 'y', 0 },
	{ 'W', 'e', 'd', 'n', 'e', 's', 'd', 'a', 'y', 0 },
	{ 'T', 'h', 'u', 'r', 's', 'd', 'a', 'y', 0 },
	{ 'F', 'r', 'i', 'd', 'a', 'y', 0 },
	{ 'S', 'a', 't', 'u', 'r', 'd', 'a', 'y', 0 }
    };
    int day;
    day = (int) exsltDateDayInWeek(dateTime);
    if((day < 1) || (day > 7))
      day = 0;
    return dayNames[day];
}
