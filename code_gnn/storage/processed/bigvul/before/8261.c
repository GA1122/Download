XRecordCreateContext(Display *dpy, int datum_flags,
		     XRecordClientSpec *clients, int nclients,
		     XRecordRange **ranges, int nranges)
{
    XExtDisplayInfo 	*info = find_display (dpy);
    register xRecordCreateContextReq 	*req;
    int			clen = 4 * nclients;

    XRecordCheckExtension (dpy, info, 0);
    LockDisplay(dpy);
    GetReq(RecordCreateContext, req);

    req->reqType = info->codes->major_opcode;
    req->recordReqType = X_RecordCreateContext;
    req->context = XAllocID(dpy);
    req->length += (nclients * 4 +
		    nranges * SIZEOF(xRecordRange)) >> 2;
    req->elementHeader = datum_flags;
    req->nClients = nclients;
    req->nRanges = nranges;

    Data32(dpy, (long *)clients, clen);
    SendRange(dpy, ranges, nranges);

    UnlockDisplay(dpy);
    SyncHandle();
    return req->context;
}
