str_to_bool(char *value, int old)
{
    if (value == NULL)
	return 1;
    switch (TOLOWER(*value)) {
    case '0':
    case 'f':			 
    case 'n':			 
    case 'u':			 
	return 0;
    case 'o':
	if (TOLOWER(value[1]) == 'f')	 
	    return 0;
	return 1;		 
    case 't':
	if (TOLOWER(value[1]) == 'o')	 
	    return !old;
	return 1;		 
    case '!':
    case 'r':			 
    case 'x':			 
	return !old;
    }
    return 1;
}
