followTab(TabBuffer * tab)
{
    Buffer *buf;
    Anchor *a;

#ifdef USE_IMAGE
    a = retrieveCurrentImg(Currentbuf);
    if (!(a && a->image && a->image->map))
#endif
	a = retrieveCurrentAnchor(Currentbuf);
    if (a == NULL)
	return;

    if (tab == CurrentTab) {
	check_target = FALSE;
	followA();
	check_target = TRUE;
	return;
    }
    _newT();
    buf = Currentbuf;
    check_target = FALSE;
    followA();
    check_target = TRUE;
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