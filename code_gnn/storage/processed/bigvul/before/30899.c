static void filename_to_taskname(char *tcomm, const char *fn, unsigned int len)
{
	int i, ch;

	 
	for (i = 0; (ch = *(fn++)) != '\0';) {
		if (ch == '/')
			i = 0;  
		else
			if (i < len - 1)
				tcomm[i++] = ch;
	}
	tcomm[i] = '\0';
}
