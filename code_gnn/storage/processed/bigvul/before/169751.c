_exsltDateAddDuration (exsltDateValPtr x, exsltDateValPtr y)
{
    exsltDateValPtr ret;

    if ((x == NULL) || (y == NULL))
        return NULL;

    ret = exsltDateCreateDate(XS_DURATION);
    if (ret == NULL)
        return NULL;

    if (_exsltDateAddDurCalc(ret, x, y))
        return ret;

    exsltDateFreeDate(ret);
    return NULL;
}
