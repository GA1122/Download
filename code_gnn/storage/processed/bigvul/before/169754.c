_exsltDateDifference (exsltDateValPtr x, exsltDateValPtr y, int flag)
{
    exsltDateValPtr ret;

    if ((x == NULL) || (y == NULL))
        return NULL;

    if (((x->type < XS_GYEAR) || (x->type > XS_DATETIME)) ||
        ((y->type < XS_GYEAR) || (y->type > XS_DATETIME)))
        return NULL;

    exsltDateNormalize(x);
    exsltDateNormalize(y);

     
    if (x->type != y->type) {
        if (x->type < y->type) {
            _exsltDateTruncateDate(y, x->type);
        } else {
            _exsltDateTruncateDate(x, y->type);
        }
    }

    ret = exsltDateCreateDate(XS_DURATION);
    if (ret == NULL)
        return NULL;

    if (((x->type == XS_GYEAR) || (x->type == XS_GYEARMONTH)) && (!flag)) {
         
        ret->value.dur.mon = ((y->value.date.year * 12) + y->value.date.mon) -
                             ((x->value.date.year * 12) + x->value.date.mon);
	 
    } else {
        ret->value.dur.day  = _exsltDateCastYMToDays(y) -
                              _exsltDateCastYMToDays(x);
        ret->value.dur.day += y->value.date.day - x->value.date.day;
        ret->value.dur.sec  = TIME_TO_NUMBER(y) - TIME_TO_NUMBER(x);
	if (ret->value.dur.day > 0.0 && ret->value.dur.sec < 0.0) {
	    ret->value.dur.day -= 1;
	    ret->value.dur.sec = ret->value.dur.sec + SECS_PER_DAY;
	} else if (ret->value.dur.day < 0.0 && ret->value.dur.sec > 0.0) {
	    ret->value.dur.day += 1;
	    ret->value.dur.sec = ret->value.dur.sec - SECS_PER_DAY;
	}
    }

    return ret;
}
