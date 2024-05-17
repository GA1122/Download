XRecordGetContext(Display *dpy, XRecordContext context,
		  XRecordState **state_return)
{
    XExtDisplayInfo 	*info = find_display (dpy);
    register 		xRecordGetContextReq   	*req;
    xRecordGetContextReply 	rep;
    unsigned int	count, i, rn;
    xRecordRange   	xrange;
    xRecordClientInfo   xclient_inf;
    XRecordState	*ret;

    XRecordCheckExtension (dpy, info, 0);
    LockDisplay(dpy);
    GetReq(RecordGetContext, req);
    req->reqType = info->codes->major_opcode;
    req->recordReqType = X_RecordGetContext;
    req->context = context;
    if (!_XReply(dpy,(xReply *)&rep, 0, False)) {
	UnlockDisplay(dpy);
	SyncHandle();
	return 0;
    }
    count = rep.nClients;

    ret = (XRecordState*)Xmalloc(sizeof(XRecordState));
    if (!ret) {
	_XEatDataWords (dpy, rep.length);
	UnlockDisplay(dpy);
	SyncHandle();
	return 0;
    }

    ret->enabled = rep.enabled;
    ret->datum_flags = rep.elementHeader;
    ret->nclients = count;

    if (count)
    {
	XRecordClientInfo	**client_inf = NULL;
	XRecordClientInfo	*client_inf_str = NULL;

	if (count < (INT_MAX / sizeof(XRecordClientInfo))) {
	    client_inf = Xcalloc(count, sizeof(XRecordClientInfo *));
	    if (client_inf != NULL)
		client_inf_str = Xmalloc(count * sizeof(XRecordClientInfo));
	}
	ret->client_info = client_inf;
        if (!client_inf || !client_inf_str)
        {
	   free(client_inf);
	   _XEatDataWords (dpy, rep.length);
	   UnlockDisplay(dpy);
	   XRecordFreeState(ret);
	   SyncHandle();
	   return 0;
        }
        for(i = 0; i < count; i++)
        {
	    client_inf[i] = &(client_inf_str[i]);
            _XRead(dpy, (char *)&xclient_inf, (long)sizeof(xRecordClientInfo));
            client_inf_str[i].client = xclient_inf.clientResource;
            client_inf_str[i].nranges = xclient_inf.nRanges;

	    if (xclient_inf.nRanges)
	    {
		XRecordRange	*ranges = NULL;

		if (xclient_inf.nRanges < (INT_MAX / sizeof(XRecordRange))) {
		    client_inf_str[i].ranges =
			Xcalloc(xclient_inf.nRanges, sizeof(XRecordRange *));
		    if (client_inf_str[i].ranges != NULL)
			ranges =
			    Xmalloc(xclient_inf.nRanges * sizeof(XRecordRange));
		}
		else
		    client_inf_str[i].ranges = NULL;

		if (!client_inf_str[i].ranges || !ranges) {
		     
		    UnlockDisplay(dpy);
		    XRecordFreeState(ret);
		    SyncHandle();
		    return 0;
		}
		for (rn=0; rn<xclient_inf.nRanges; rn++) {
		    client_inf_str[i].ranges[rn] = &(ranges[rn]);
		    _XRead(dpy, (char *)&xrange, (long)sizeof(xRecordRange));
		    WireToLibRange(&xrange, &(ranges[rn]));
		}
	    } else {
		client_inf_str[i].ranges = NULL;
	    }
        }
    } else {
	ret->client_info = NULL;
    }

    *state_return = ret;

    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}
