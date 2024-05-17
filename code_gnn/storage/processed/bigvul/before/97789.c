xmlXPathStringHash(const xmlChar * string) {
    if (string == NULL)
	return((unsigned int) 0);
    if (string[0] == 0)
	return(0);
    return(((unsigned int) string[0]) +
	   (((unsigned int) string[1]) << 8));
}
