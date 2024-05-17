str_to_color(char *value)
{
    if (value == NULL)
	return 8;		 
    switch (TOLOWER(*value)) {
    case '0':
	return 0;		 
    case '1':
    case 'r':
	return 1;		 
    case '2':
    case 'g':
	return 2;		 
    case '3':
    case 'y':
	return 3;		 
    case '4':
	return 4;		 
    case '5':
    case 'm':
	return 5;		 
    case '6':
    case 'c':
	return 6;		 
    case '7':
    case 'w':
	return 7;		 
    case '8':
    case 't':
	return 8;		 
    case 'b':
	if (!strncasecmp(value, "blu", 3))
	    return 4;		 
	else
	    return 0;		 
    }
    return 8;			 
}
