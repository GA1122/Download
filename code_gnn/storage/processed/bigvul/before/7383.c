eatsize(const char **p)
{
	const char *l = *p;

	if (LOWCASE(*l) == 'u') 
		l++;

	switch (LOWCASE(*l)) {
	case 'l':     
	case 's':     
	case 'h':     
	case 'b':     
	case 'c':     
		l++;
		 
	default:
		break;
	}

	*p = l;
}
