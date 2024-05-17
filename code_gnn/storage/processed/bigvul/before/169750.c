_exsltDateAddDurCalc (exsltDateValPtr ret, exsltDateValPtr x,
		      exsltDateValPtr y)
{
    long carry;

     
    ret->value.dur.mon = x->value.dur.mon + y->value.dur.mon;

     
    ret->value.dur.sec = x->value.dur.sec + y->value.dur.sec;
    carry = (long)FQUOTIENT(ret->value.dur.sec, SECS_PER_DAY);
    if (ret->value.dur.sec != 0.0) {
        ret->value.dur.sec = MODULO(ret->value.dur.sec, SECS_PER_DAY);
	 
	if ((carry < 0) && (ret->value.dur.sec != 0)) {
	     
	    ret->value.dur.sec = ret->value.dur.sec - SECS_PER_DAY;
	    carry++;
	}
    }

     
    ret->value.dur.day = x->value.dur.day + y->value.dur.day + carry;

     
    if ((((ret->value.dur.day > 0) || (ret->value.dur.sec > 0)) &&
         (ret->value.dur.mon < 0)) ||
        (((ret->value.dur.day < 0) || (ret->value.dur.sec < 0)) &&
         (ret->value.dur.mon > 0))) {
        return 0;
    }
    return 1;
}
