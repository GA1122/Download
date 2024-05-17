exsltDateCastDateToNumber (const exsltDateValPtr dt)
{
    double ret = 0.0;

    if (dt == NULL)
        return 0.0;

    if ((dt->type & XS_GYEAR) == XS_GYEAR) {
        ret = (double)_exsltDateCastYMToDays(dt) * SECS_PER_DAY;
    }

     
    if (dt->type == XS_DURATION) {
        ret += (double)dt->value.dur.day * SECS_PER_DAY;
        ret += dt->value.dur.sec;
    } else {
        ret += (double)dt->value.date.day * SECS_PER_DAY;
         
        ret += TIME_TO_NUMBER(dt);
    }


    return ret;
}
