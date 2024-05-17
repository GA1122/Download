srch_nxtprv(int reverse)
{
    int result;
     
    static int (*routine[2]) (Buffer *, char *) = {
	forwardSearch, backwardSearch
    };
     

    if (searchRoutine == NULL) {
	 
	disp_message("No previous regular expression", TRUE);
	return;
    }
    if (reverse != 0)
	reverse = 1;
    if (searchRoutine == backwardSearch)
	reverse ^= 1;
    if (reverse == 0)
	Currentbuf->pos += 1;
    result = srchcore(SearchString, routine[reverse]);
    if (result & SR_FOUND)
	clear_mark(Currentbuf->currentLine);
    else {
	if (reverse == 0)
	    Currentbuf->pos -= 1;
    }
    displayBuffer(Currentbuf, B_NORMAL);
    disp_srchresult(result, (reverse ? "Backward: " : "Forward: "),
		    SearchString);
}