exsltDateDayAbbreviation (const xmlChar *dateTime)
{
    static const xmlChar dayAbbreviations[8][4] = {
        { 0 },
	{ 'S', 'u', 'n', 0 },
	{ 'M', 'o', 'n', 0 },
	{ 'T', 'u', 'e', 0 },
	{ 'W', 'e', 'd', 0 },
	{ 'T', 'h', 'u', 0 },
	{ 'F', 'r', 'i', 0 },
	{ 'S', 'a', 't', 0 }
    };
    int day;
    day = (int) exsltDateDayInWeek(dateTime);
    if((day < 1) || (day > 7))
      day = 0;
    return dayAbbreviations[day];
}
