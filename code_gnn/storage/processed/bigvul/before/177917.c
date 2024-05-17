 char **XGetFontPath(
     register Display *dpy,
     int *npaths)	 
 {
 	xGetFontPathReply rep;
 	unsigned long nbytes = 0;
 	char **flist = NULL;
 	char *ch = NULL;
 	char *chend;
 	int count = 0;
 	register unsigned i;
 	register int length;
 	_X_UNUSED register xReq *req;
 
 	LockDisplay(dpy);
 	GetEmptyReq (GetFontPath, req);
 	(void) _XReply (dpy, (xReply *) &rep, 0, xFalse);
 
 	if (rep.nPaths) {
 	    flist = Xmalloc(rep.nPaths * sizeof (char *));
 	    if (rep.length < (INT_MAX >> 2)) {
 		nbytes = (unsigned long) rep.length << 2;
 		ch = Xmalloc (nbytes + 1);
                  
 	    }
 
 	    if ((! flist) || (! ch)) {
 		Xfree(flist);
 		Xfree(ch);
 		_XEatDataWords(dpy, rep.length);
 		UnlockDisplay(dpy);
 		SyncHandle();
 		return (char **) NULL;
 	    }
 
 	    _XReadPad (dpy, ch, nbytes);
 	     
  	    chend = ch + nbytes;
	    length = *ch;
// 	    length = *(unsigned char *)ch;
  	    for (i = 0; i < rep.nPaths; i++) {
  		if (ch + length < chend) {
  		    flist[i] = ch+1;   
  		    ch += length + 1;  
		    length = *ch;
// 		    length = *(unsigned char *)ch;
  		    *ch = '\0';  
  		    count++;
  		} else
 		    flist[i] = NULL;
 	    }
 	}
 	*npaths = count;
 	UnlockDisplay(dpy);
 	SyncHandle();
 	return (flist);
 }