XRecordUnregisterClients(Display *dpy, XRecordContext context,
			 XRecordClientSpec *clients, int nclients)
{
    XExtDisplayInfo *info = find_display (dpy);
    register xRecordUnregisterClientsReq 	*req;
    int			clen = 4 * nclients;

    XRecordCheckExtension (dpy, info, 0);
    LockDisplay(dpy);
    GetReq(RecordUnregisterClients, req);

    req->reqType = info->codes->major_opcode;
    req->recordReqType = X_RecordUnregisterClients;
    req->context = context;
    req->length += nclients;
    req->nClients = nclients;

    Data32(dpy, (long *)clients, clen);

    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}
