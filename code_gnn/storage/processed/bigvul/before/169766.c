exsltDateDateTime (void)
{
    xmlChar *ret = NULL;
#ifdef WITH_TIME
    exsltDateValPtr cur;

    cur = exsltDateCurrent();
    if (cur != NULL) {
	ret = exsltDateFormatDateTime(&(cur->value.date));
	exsltDateFreeDate(cur);
    }
#endif

    return ret;
}
