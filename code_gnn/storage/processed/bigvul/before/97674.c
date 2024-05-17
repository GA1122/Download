xmlXPathFormatNumber(double number, char buffer[], int buffersize)
{
    switch (xmlXPathIsInf(number)) {
    case 1:
	if (buffersize > (int)sizeof("Infinity"))
	    snprintf(buffer, buffersize, "Infinity");
	break;
    case -1:
	if (buffersize > (int)sizeof("-Infinity"))
	    snprintf(buffer, buffersize, "-Infinity");
	break;
    default:
	if (xmlXPathIsNaN(number)) {
	    if (buffersize > (int)sizeof("NaN"))
		snprintf(buffer, buffersize, "NaN");
	} else if (number == 0 && xmlXPathGetSign(number) != 0) {
	    snprintf(buffer, buffersize, "0");
	} else if (number == ((int) number)) {
	    char work[30];
	    char *ptr, *cur;
	    int value = (int) number;

            ptr = &buffer[0];
	    if (value == 0) {
		*ptr++ = '0';
	    } else {
		snprintf(work, 29, "%d", value);
		cur = &work[0];
		while ((*cur) && (ptr - buffer < buffersize)) {
		    *ptr++ = *cur++;
		}
	    }
	    if (ptr - buffer < buffersize) {
		*ptr = 0;
	    } else if (buffersize > 0) {
		ptr--;
		*ptr = 0;
	    }
	} else {
	     
	    char work[DBL_DIG + EXPONENT_DIGITS + 3 + LOWER_DOUBLE_EXP];
	    int integer_place, fraction_place;
	    char *ptr;
	    char *after_fraction;
	    double absolute_value;
	    int size;

	    absolute_value = fabs(number);

	     
	    if ( ((absolute_value > UPPER_DOUBLE) ||
		  (absolute_value < LOWER_DOUBLE)) &&
		 (absolute_value != 0.0) ) {
		 
		integer_place = DBL_DIG + EXPONENT_DIGITS + 1;
		fraction_place = DBL_DIG - 1;
		size = snprintf(work, sizeof(work),"%*.*e",
			 integer_place, fraction_place, number);
		while ((size > 0) && (work[size] != 'e')) size--;

	    }
	    else {
		 
		if (absolute_value > 0.0) {
		    integer_place = (int)log10(absolute_value);
		    if (integer_place > 0)
		        fraction_place = DBL_DIG - integer_place - 1;
		    else
		        fraction_place = DBL_DIG - integer_place;
		} else {
		    fraction_place = 1;
		}
		size = snprintf(work, sizeof(work), "%0.*f",
				fraction_place, number);
	    }

	     
	    after_fraction = work + size;
	    ptr = after_fraction;
	    while (*(--ptr) == '0')
		;
	    if (*ptr != '.')
	        ptr++;
	    while ((*ptr++ = *after_fraction++) != 0);

	     
	    size = strlen(work) + 1;
	    if (size > buffersize) {
		work[buffersize - 1] = 0;
		size = buffersize;
	    }
	    memmove(buffer, work, size);
	}
	break;
    }
}