exsltDateParseDuration (const xmlChar *duration)
{
    const xmlChar  *cur = duration;
    exsltDateValPtr dur;
    int isneg = 0;
    unsigned int seq = 0;

    if (duration == NULL)
	return NULL;

    if (*cur == '-') {
        isneg = 1;
        cur++;
    }

     
    if (*cur++ != 'P')
	return NULL;

    dur = exsltDateCreateDate(XS_DURATION);
    if (dur == NULL)
	return NULL;

    while (*cur != 0) {
        double         num;
        int            num_type = 0;   
        const xmlChar  desig[] = {'Y', 'M', 'D', 'H', 'M', 'S'};
        const double   multi[] = { 0.0, 0.0, 86400.0, 3600.0, 60.0, 1.0, 0.0};

         
        if (seq >= sizeof(desig))
            goto error;

         
        if (*cur == 'T') {
            if (seq <= 3) {
                seq = 3;
                cur++;
            } else
                return NULL;
        } else if (seq == 3)
            goto error;

         
        PARSE_NUM(num, cur, num_type);

        if ((num_type == -1) || (*cur == 0))
            goto error;

         
        while (seq < sizeof(desig)) {
            if (*cur == desig[seq]) {

                 
                if ((num_type != 0) && (seq < (sizeof(desig)-1)))
                    goto error;

                switch (seq) {
                    case 0:
                        dur->value.dur.mon = (long)num * 12;
                        break;
                    case 1:
                        dur->value.dur.mon += (long)num;
                        break;
                    default:
                         
                        dur->value.dur.sec += num * multi[seq];
                        seq++;
                        break;
                }

                break;           
            }
             
            if (++seq == 3)
                goto error;
        }
        cur++;
    }

    if (isneg) {
        dur->value.dur.mon = -dur->value.dur.mon;
        dur->value.dur.day = -dur->value.dur.day;
        dur->value.dur.sec = -dur->value.dur.sec;
    }

#ifdef DEBUG_EXSLT_DATE
    xsltGenericDebug(xsltGenericDebugContext,
		     "Parsed duration %f\n", dur->value.dur.sec);
#endif

    return dur;

error:
    if (dur != NULL)
	exsltDateFreeDate(dur);
    return NULL;
}
