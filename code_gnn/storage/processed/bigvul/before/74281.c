int parse_number(char *arg, int *res)
{
	char *b;
	long number = strtol(arg, &b, 10);

	 
	if(*b != '\0')
		return 0;

	 
	if(number == LONG_MIN || number == LONG_MAX)
		return 0;

	 
	if(number < 0)
		return 0;

	 
	if(number > INT_MAX)
		return 0;

	*res = number;
	return 1;
}
