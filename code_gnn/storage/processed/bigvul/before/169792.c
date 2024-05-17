exsltDateMonthName (const xmlChar *dateTime)
{
    static const xmlChar monthNames[13][10] = {
        { 0 },
	{ 'J', 'a', 'n', 'u', 'a', 'r', 'y', 0 },
	{ 'F', 'e', 'b', 'r', 'u', 'a', 'r', 'y', 0 },
	{ 'M', 'a', 'r', 'c', 'h', 0 },
	{ 'A', 'p', 'r', 'i', 'l', 0 },
	{ 'M', 'a', 'y', 0 },
	{ 'J', 'u', 'n', 'e', 0 },
	{ 'J', 'u', 'l', 'y', 0 },
	{ 'A', 'u', 'g', 'u', 's', 't', 0 },
	{ 'S', 'e', 'p', 't', 'e', 'm', 'b', 'e', 'r', 0 },
	{ 'O', 'c', 't', 'o', 'b', 'e', 'r', 0 },
	{ 'N', 'o', 'v', 'e', 'm', 'b', 'e', 'r', 0 },
	{ 'D', 'e', 'c', 'e', 'm', 'b', 'e', 'r', 0 }
    };
    int month;
    month = (int) exsltDateMonthInYear(dateTime);
    if (!VALID_MONTH(month))
      month = 0;
    return monthNames[month];
}
