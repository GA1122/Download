tabURL0(TabBuffer * tab, char *prompt, int relative)
{
    Buffer *buf;

    if (tab == CurrentTab) {
	goURL0(prompt, relative);
	return;
    }
    _newT();
    buf = Currentbuf;
    goURL0(prompt, relative);
    if (tab == NULL) {
	if (buf != Currentbuf)
	    delBuffer(buf);
	else
	    deleteTab(CurrentTab);
    }
    else if (buf != Currentbuf) {
	 
	Buffer *c, *p;

	c = Currentbuf;
	p = prevBuffer(c, buf);
	p->nextBuffer = NULL;
	Firstbuf = buf;
	deleteTab(CurrentTab);
	CurrentTab = tab;
	for (buf = p; buf; buf = p) {
	    p = prevBuffer(c, buf);
	    pushBuffer(buf);
	}
    }
    displayBuffer(Currentbuf, B_FORCE_REDRAW);
}