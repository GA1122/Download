gotoLabel(char *label)
{
    Buffer *buf;
    Anchor *al;
    int i;

    al = searchURLLabel(Currentbuf, label);
    if (al == NULL) {
	 
	disp_message(Sprintf("%s is not found", label)->ptr, TRUE);
	return;
    }
    buf = newBuffer(Currentbuf->width);
    copyBuffer(buf, Currentbuf);
    for (i = 0; i < MAX_LB; i++)
	buf->linkBuffer[i] = NULL;
    buf->currentURL.label = allocStr(label, -1);
    pushHashHist(URLHist, parsedURL2Str(&buf->currentURL)->ptr);
    (*buf->clone)++;
    pushBuffer(buf);
    gotoLine(Currentbuf, al->start.line);
    if (label_topline)
	Currentbuf->topLine = lineSkip(Currentbuf, Currentbuf->topLine,
				       Currentbuf->currentLine->linenumber
				       - Currentbuf->topLine->linenumber,
				       FALSE);
    Currentbuf->pos = al->start.pos;
    arrangeCursor(Currentbuf);
    displayBuffer(Currentbuf, B_FORCE_REDRAW);
    return;
}