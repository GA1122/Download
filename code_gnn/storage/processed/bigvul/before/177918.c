 char **XListExtensions(
     register Display *dpy,
     int *nextensions)	 
 {
 	xListExtensionsReply rep;
 	char **list = NULL;
 	char *ch = NULL;
 	char *chend;
 	int count = 0;
 	register unsigned i;
 	register int length;
 	_X_UNUSED register xReq *req;
 	unsigned long rlen = 0;
 
 	LockDisplay(dpy);
 	GetEmptyReq (ListExtensions, req);
 
 	if (! _XReply (dpy, (xReply *) &rep, 0, xFalse)) {
 	    UnlockDisplay(dpy);
 	    SyncHandle();
 	    return (char **) NULL;
 	}
 
 	if (rep.nExtensions) {
 	    list = Xmalloc (rep.nExtensions * sizeof (char *));
 	    if (rep.length > 0 && rep.length < (INT_MAX >> 2)) {
 		rlen = rep.length << 2;
 		ch = Xmalloc (rlen + 1);
                  
 	    }
 
 	    if ((!list) || (!ch)) {
 		Xfree(list);
 		Xfree(ch);
 		_XEatDataWords(dpy, rep.length);
 		UnlockDisplay(dpy);
 		SyncHandle();
 		return (char **) NULL;
 	    }
 
 	    _XReadPad (dpy, ch, rlen);
 	     
  	    chend = ch + rlen;
	    length = *ch;
// 	    length = *(unsigned char *)ch;
  	    for (i = 0; i < rep.nExtensions; i++) {
  		if (ch + length < chend) {
  		    list[i] = ch+1;   
  		    ch += length + 1;  
		    length = *ch;
// 		    length = *(unsigned char *)ch;
  		    *ch = '\0';  
  		    count++;
  		} else
 		    list[i] = NULL;
 	    }
 	}
 
 	*nextensions = count;
 	UnlockDisplay(dpy);
 	SyncHandle();
 	return (list);
 }