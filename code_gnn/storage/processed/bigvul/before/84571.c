set_param_option(char *option)
{
    Str tmp = Strnew();
    char *p = option, *q;

    while (*p && !IS_SPACE(*p) && *p != '=')
	Strcat_char(tmp, *p++);
    while (*p && IS_SPACE(*p))
	p++;
    if (*p == '=') {
	p++;
	while (*p && IS_SPACE(*p))
	    p++;
    }
    Strlower(tmp);
    if (set_param(tmp->ptr, p))
	goto option_assigned;
    q = tmp->ptr;
    if (!strncmp(q, "no", 2)) {	 
	q += 2;
	if (*q == '-' || *q == '_')
	    q++;
    }
    else if (tmp->ptr[0] == '-')	 
	q++;
    else
	return 0;
    if (set_param(q, "0"))
	goto option_assigned;
    return 0;
  option_assigned:
    return 1;
}