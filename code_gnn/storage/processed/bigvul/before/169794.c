exsltDateNormalize (exsltDateValPtr dt)
{
    exsltDateValPtr dur, tmp;

    if (dt == NULL)
        return;

    if (((dt->type & XS_TIME) != XS_TIME) && (dt->value.date.tzo == 0))
        return;

    dur = exsltDateCreateDate(XS_DURATION);
    if (dur == NULL)
        return;

    tmp = _exsltDateAdd(dt, dur);
    if (tmp == NULL)
        return;

    memcpy(dt, tmp, sizeof(exsltDateVal));

    exsltDateFreeDate(tmp);
    exsltDateFreeDate(dur);

    dt->value.date.tzo = 0;
}
