 XListFonts(
 register Display *dpy,
 _Xconst char *pattern,   
 int maxNames,
 int *actualCount)	 
 {
     register long nbytes;
     register unsigned i;
     register int length;
     char **flist = NULL;
     char *ch = NULL;
     char *chend;
     int count = 0;
     xListFontsReply rep;
     register xListFontsReq *req;
     unsigned long rlen = 0;
 
     LockDisplay(dpy);
     GetReq(ListFonts, req);
     req->maxNames = maxNames;
     nbytes = req->nbytes = pattern ? strlen (pattern) : 0;
     req->length += (nbytes + 3) >> 2;
     _XSend (dpy, pattern, nbytes);
      
 
     if (!_XReply (dpy, (xReply *)&rep, 0, xFalse)) {
 	*actualCount = 0;
 	UnlockDisplay(dpy);
 	SyncHandle();
 	return (char **) NULL;
     }
  
      if (rep.nFonts) {
  	flist = Xmalloc (rep.nFonts * sizeof(char *));
	if (rep.length < (INT_MAX >> 2)) {
// 	if (rep.length > 0 && rep.length < (INT_MAX >> 2)) {
  	    rlen = rep.length << 2;
  	    ch = Xmalloc(rlen + 1);
  	     
 	}
 
 	if ((! flist) || (! ch)) {
 	    Xfree(flist);
 	    Xfree(ch);
 	    _XEatDataWords(dpy, rep.length);
 	    *actualCount = 0;
 	    UnlockDisplay(dpy);
 	    SyncHandle();
 	    return (char **) NULL;
 	}
 
 	_XReadPad (dpy, ch, rlen);
 	 
 	chend = ch + (rlen + 1);
 	length = *(unsigned char *)ch;
 	*ch = 1;  
 	for (i = 0; i < rep.nFonts; i++) {
  	    if (ch + length < chend) {
  		flist[i] = ch + 1;   
  		ch += length + 1;   
		length = *(unsigned char *)ch;
		*ch = '\0';   
		count++;
	    } else
		flist[i] = NULL;
// 		if (ch <= chend) {
// 		    length = *(unsigned char *)ch;
// 		    *ch = '\0';   
// 		    count++;
// 		} else {
//                     Xfree(flist);
//                     flist = NULL;
//                     count = 0;
//                     break;
// 		}
// 	    } else {
//                 Xfree(flist);
//                 flist = NULL;
//                 count = 0;
//                 break;
//             }
  	}
      }
      *actualCount = count;
 			for (names = list+1; *names; names++)
 				Xfree (*names);
 		}