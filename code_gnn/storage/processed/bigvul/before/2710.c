pch_swap (void)
{
    char **tp_line;		 
    size_t *tp_len;		 
    char *tp_char;		 
    lin i;
    lin n;
    bool blankline = false;
    char *s;

    i = p_first;
    p_first = p_newfirst;
    p_newfirst = i;

     

    tp_line = p_line;
    tp_len = p_len;
    tp_char = p_Char;
    p_line = 0;	 
    p_len = 0;
    p_Char = 0;
    set_hunkmax();
    if (!p_line || !p_len || !p_Char) {
	free (p_line);
	p_line = tp_line;
	free (p_len);
	p_len = tp_len;
	free (p_Char);
	p_Char = tp_char;
	return false;		 
    }

     

    i = p_ptrn_lines + 1;
    if (tp_char[i] == '\n') {		 
	blankline = true;
	i++;
    }
    if (p_efake >= 0) {			 
	if (p_efake <= i)
	    n = p_end - i + 1;
	else
	    n = -i;
	p_efake += n;
	p_bfake += n;
    }
    for (n=0; i <= p_end; i++,n++) {
	p_line[n] = tp_line[i];
	p_Char[n] = tp_char[i];
	if (p_Char[n] == '+')
	    p_Char[n] = '-';
	p_len[n] = tp_len[i];
    }
    if (blankline) {
	i = p_ptrn_lines + 1;
	p_line[n] = tp_line[i];
	p_Char[n] = tp_char[i];
	p_len[n] = tp_len[i];
	n++;
    }
    assert(p_Char[0] == '=');
    p_Char[0] = '*';
    for (s=p_line[0]; *s; s++)
	if (*s == '-')
	    *s = '*';

     

    assert(tp_char[0] == '*');
    tp_char[0] = '=';
    for (s=tp_line[0]; *s; s++)
	if (*s == '*')
	    *s = '-';
    for (i=0; n <= p_end; i++,n++) {
	p_line[n] = tp_line[i];
	p_Char[n] = tp_char[i];
	if (p_Char[n] == '-')
	    p_Char[n] = '+';
	p_len[n] = tp_len[i];
    }
    assert(i == p_ptrn_lines + 1);
    i = p_ptrn_lines;
    p_ptrn_lines = p_repl_lines;
    p_repl_lines = i;
    p_Char[p_end + 1] = '^';
    free (tp_line);
    free (tp_len);
    free (tp_char);
    return true;
}