static int _assemble_line(FILE *f, char *buffer, int buf_len)
{
    char *p = buffer;
    char *s, *os;
    int used = 0;

     

    D(("called."));
    for (;;) {
	if (used >= buf_len) {
	     
	    D(("_assemble_line: overflow"));
	    return -1;
	}
	if (fgets(p, buf_len - used, f) == NULL) {
	    if (used) {
		 
		return -1;
	    } else {
		 
		return 0;
	    }
	}

	 

	s = p + strspn(p, " \n\t");
	if (*s && (*s != '#')) {
	    os = s;

	     

	    while (*s && *s != '#')
		 ++s;
	    if (*s == '#') {
		 *s = '\0';
		 used += strlen(os);
		 break;                 
	    }

	    s = os;

	     

	    s += strlen(s);
	    while (s > os && ((*--s == ' ') || (*s == '\t')
			      || (*s == '\n')));

	     
	    if (*s == '\\') {
		*s = '\0';               
		used += strlen(os);
		p = s;                   
	    } else {
		 
		used += strlen(os);
		break;                   
	    }

	} else {
	     
	     
	}
    }

    return used;
}
