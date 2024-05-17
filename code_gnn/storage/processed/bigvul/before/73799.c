static void ToUpperCase(
	 
	char *s)
{
	while (*s) {
		*s = (char)toupper(*s);
		++s;
	}
}
