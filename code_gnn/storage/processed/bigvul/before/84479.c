cur_real_linenumber(Buffer *buf)
{
    Line *l, *cur = buf->currentLine;
    int n;

    if (!cur)
	return 1;
    n = cur->real_linenumber ? cur->real_linenumber : 1;
    for (l = buf->firstLine; l && l != cur && l->real_linenumber == 0; l = l->next) {	 
	if (l->bpos == 0)
	    n++;
    }
    return n;
}