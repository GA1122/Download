CreateColors(
    char		**dataptr,
    unsigned int	 *data_size,
    XpmColor		 *colors,
    unsigned int	  ncolors,
    unsigned int	  cpp)
{
    char buf[BUFSIZ];
    unsigned int a, key, l;
    char *s, *s2;
    char **defaults;

     
    for (a = 0; a < ncolors; a++, colors++, dataptr++) {

	defaults = (char **) colors;
	if(sizeof(buf) <= cpp)
	    return(XpmNoMemory);
	strncpy(buf, *defaults++, cpp);
	s = buf + cpp;

	if(sizeof(buf) <= (s-buf))
		return XpmNoMemory;

	for (key = 1; key <= NKEYS; key++, defaults++) {
	    if ((s2 = *defaults)) {
#ifndef VOID_SPRINTF
		s +=
#endif
		 
			snprintf(s, sizeof(buf)-(s-buf), "\t%s %s", xpmColorKeys[key - 1], s2);
#ifdef VOID_SPRINTF
		s += strlen(s);
#endif
		 
		if(sizeof(buf) < (s-buf))
			return XpmNoMemory;
	    }
	}
	 
	l = s - buf + 1;
	s = (char *) XpmMalloc(l);
	if (!s)
	    return (XpmNoMemory);
	*data_size += l;
	*dataptr = strcpy(s, buf);
    }
    return (XpmSuccess);
}
