exsltDateMonthAbbreviation (const xmlChar *dateTime)
{
    static const xmlChar monthAbbreviations[13][4] = {
        { 0 },
	{ 'J', 'a', 'n', 0 },
	{ 'F', 'e', 'b', 0 },
	{ 'M', 'a', 'r', 0 },
	{ 'A', 'p', 'r', 0 },
	{ 'M', 'a', 'y', 0 },
	{ 'J', 'u', 'n', 0 },
	{ 'J', 'u', 'l', 0 },
	{ 'A', 'u', 'g', 0 },
	{ 'S', 'e', 'p', 0 },
	{ 'O', 'c', 't', 0 },
	{ 'N', 'o', 'v', 0 },
	{ 'D', 'e', 'c', 0 }
    };
    int month;
    month = (int) exsltDateMonthInYear(dateTime);
    if(!VALID_MONTH(month))
      month = 0;
    return monthAbbreviations[month];
}
