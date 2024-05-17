interpret_rc(FILE * f)
{
    Str line;
    Str tmp;
    char *p;

    for (;;) {
	line = Strfgets(f);
	if (line->length == 0)		 
	    break;
	Strchop(line);
	if (line->length == 0)		 
	    continue;
	Strremovefirstspaces(line);
	if (line->ptr[0] == '#')	 
	    continue;
	tmp = Strnew();
	p = line->ptr;
	while (*p && !IS_SPACE(*p))
	    Strcat_char(tmp, *p++);
	while (*p && IS_SPACE(*p))
	    p++;
	Strlower(tmp);
	set_param(tmp->ptr, p);
    }
}
