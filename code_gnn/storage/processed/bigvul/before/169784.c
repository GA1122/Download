exsltDateFreeDate (exsltDateValPtr date) {
    if (date == NULL)
	return;

    xmlFree(date);
}
