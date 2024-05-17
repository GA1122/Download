GetEntries(Entries *entries, Buffer *buff, int bequiet)
{
    register char *line, *colon, *temp, *str;
    Entry entry;
    register int length;
    int lineno = 0;
    int lines_skipped;

    str = buff->buff;
    if (!str) return;
    for ( ; str < buff->buff + buff->used;
	  str = line + 1, lineno += lines_skipped) {
	line = FindFirst(str, '\n', &lines_skipped);
	lineno++;
	if (!line)
	    line = buff->buff + buff->used;
	if (*str == '!')
	    continue;
	if (*str == '\n')
	    continue;
	if (!bequiet && *str == '#') {
	    int dummy;
	    if (sscanf (str, "# %d", &dummy) == 1 ||
		sscanf (str, "# line %d", &dummy) == 1)
		lineno = dummy - 1;
	    continue;
	}
	for (temp = str; 
	     *temp && *temp != '\n' && isascii(*temp) && isspace(*temp); 
	     temp++) ;
	if (!*temp || *temp == '\n') continue;

	colon = FindFirst(str, ':', NULL);
	if (!colon || colon > line) {
	    if (!bequiet && !quiet)
		fprintf (stderr, 
			 "%s: colon missing on line %d, ignoring line\n",
			 ProgramName, lineno);
	    continue;
	}

	 
	while (*str == ' ' || *str == '\t')
	    str++;
	length = colon - str;
	while (length && (str[length-1] == ' ' || str[length-1] == '\t'))
	    length--;
	temp = (char *)malloc(length + 1);
	strncpy(temp, str, length);
	temp[length] = '\0';
	entry.tag = temp;

	 
	colon++;
	while (*colon == ' ' || *colon == '\t')
	    colon++;
	length = line - colon;
	temp = (char *)malloc(length + 1);
	strncpy(temp, colon, length);
	temp[length] = '\0';
	entry.value = temp;
	entry.lineno = bequiet ? 0 : lineno;

	AddEntry(entries, &entry);
    }
}
