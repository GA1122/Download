exsltDateLeapYear (const xmlChar *dateTime)
{
    double year;

    year = exsltDateYear(dateTime);
    if (xmlXPathIsNaN(year))
	return xmlXPathNewFloat(xmlXPathNAN);

    if (IS_LEAP((long)year))
	return xmlXPathNewBoolean(1);

    return xmlXPathNewBoolean(0);
}
