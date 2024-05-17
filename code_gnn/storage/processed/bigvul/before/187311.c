 _exsltDateAdd (exsltDateValPtr dt, exsltDateValPtr dur)
 {
     exsltDateValPtr ret;
     long carry, tempdays, temp;
     exsltDateValDatePtr r, d;
     exsltDateValDurationPtr u;
 
     if ((dt == NULL) || (dur == NULL))
         return NULL;
 
     ret = exsltDateCreateDate(dt->type);
     if (ret == NULL)
         return NULL;
 
     r = &(ret->value.date);
      d = &(dt->value.date);
      u = &(dur->value.dur);
  
     
    if (d->mon == 0)
        d->mon = 1;
       
      u->sec -= (d->tzo * 60);	 
      d->tzo = 0;
  
     
    if (d->day == 0)
        d->day = 1;
       
      carry  = d->mon + u->mon;
      r->mon = (unsigned int)MODULO_RANGE(carry, 1, 13);
     carry  = (long)FQUOTIENT_RANGE(carry, 1, 13);
 
      
     r->year = d->year + carry;
     if (r->year == 0) {
         if (d->year > 0)
             r->year--;
         else
             r->year++;
     }
 
      
     r->tzo     = d->tzo;
     r->tz_flag = d->tz_flag;
 
      
     r->sec = d->sec + u->sec;
     carry  = (long)FQUOTIENT((long)r->sec, 60);
     if (r->sec != 0.0) {
         r->sec = MODULO(r->sec, 60.0);
     }
 
      
     carry += d->min;
     r->min = (unsigned int)MODULO(carry, 60);
     carry  = (long)FQUOTIENT(carry, 60);
 
      
     carry  += d->hour;
     r->hour = (unsigned int)MODULO(carry, 24);
     carry   = (long)FQUOTIENT(carry, 24);
 
      
     if ((VALID_YEAR(r->year)) && (VALID_MONTH(r->mon)) &&
                   (d->day > MAX_DAYINMONTH(r->year, r->mon)))
         tempdays = MAX_DAYINMONTH(r->year, r->mon);
     else if (d->day < 1)
         tempdays = 1;
     else
         tempdays = d->day;
 
     tempdays += u->day + carry;
 
     while (1) {
         if (tempdays < 1) {
             long tmon = (long)MODULO_RANGE((int)r->mon-1, 1, 13);
             long tyr  = r->year + (long)FQUOTIENT_RANGE((int)r->mon-1, 1, 13);
             if (tyr == 0)
                 tyr--;
 	     
 	    if (tmon < 0)
 	        tmon = 0;
 	    if (tmon > 12)
 	        tmon = 12;
             tempdays += MAX_DAYINMONTH(tyr, tmon);
             carry = -1;
         } else if (tempdays > (long)MAX_DAYINMONTH(r->year, r->mon)) {
             tempdays = tempdays - MAX_DAYINMONTH(r->year, r->mon);
             carry = 1;
         } else
             break;
 
         temp = r->mon + carry;
         r->mon = (unsigned int)MODULO_RANGE(temp, 1, 13);
         r->year = r->year + (long)FQUOTIENT_RANGE(temp, 1, 13);
         if (r->year == 0) {
             if (temp < 1)
                 r->year--;
             else
                 r->year++;
 	}
     }
 
     r->day = tempdays;
 
      
     if (ret->type != XS_DATETIME) {
         if ((r->hour) || (r->min) || (r->sec))
             ret->type = XS_DATETIME;
         else if (ret->type != XS_DATE) {
             if (r->day != 1)
                 ret->type = XS_DATE;
             else if ((ret->type != XS_GYEARMONTH) && (r->mon != 1))
                 ret->type = XS_GYEARMONTH;
         }
     }
 
     return ret;
 }