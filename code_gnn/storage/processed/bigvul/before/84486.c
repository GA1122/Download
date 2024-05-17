dispincsrch(int ch, Str buf, Lineprop *prop)
{
    static Buffer sbuf;
    static Line *currentLine;
    static int pos;
    char *str;
    int do_next_search = FALSE;

    if (ch == 0 && buf == NULL) {
	SAVE_BUFPOSITION(&sbuf);	 
	currentLine = sbuf.currentLine;
	pos = sbuf.pos;
	return -1;
    }

    str = buf->ptr;
    switch (ch) {
    case 022:			 
	searchRoutine = backwardSearch;
	do_next_search = TRUE;
	break;
    case 023:			 
	searchRoutine = forwardSearch;
	do_next_search = TRUE;
	break;

#ifdef USE_MIGEMO
    case 034:
	migemo_active = -migemo_active;
	goto done;
#endif

    default:
	if (ch >= 0)
	    return ch;		 
    }

    if (do_next_search) {
	if (*str) {
	    if (searchRoutine == forwardSearch)
		Currentbuf->pos += 1;
	    SAVE_BUFPOSITION(&sbuf);
	    if (srchcore(str, searchRoutine) == SR_NOTFOUND
		&& searchRoutine == forwardSearch) {
		Currentbuf->pos -= 1;
		SAVE_BUFPOSITION(&sbuf);
	    }
	    arrangeCursor(Currentbuf);
	    displayBuffer(Currentbuf, B_FORCE_REDRAW);
	    clear_mark(Currentbuf->currentLine);
	    return -1;
	}
	else
	    return 020;		 
    }
    else if (*str) {
	RESTORE_BUFPOSITION(&sbuf);
	arrangeCursor(Currentbuf);
	srchcore(str, searchRoutine);
	arrangeCursor(Currentbuf);
	currentLine = Currentbuf->currentLine;
	pos = Currentbuf->pos;
    }
    displayBuffer(Currentbuf, B_FORCE_REDRAW);
    clear_mark(Currentbuf->currentLine);
#ifdef USE_MIGEMO
  done:
    while (*str++ != '\0') {
	if (migemo_active > 0)
	    *prop++ |= PE_UNDER;
	else
	    *prop++ &= ~PE_UNDER;
    }
#endif
    return -1;
}