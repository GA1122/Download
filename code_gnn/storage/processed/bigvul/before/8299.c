XRRSetProviderOffloadSink(Display *dpy, XID provider,
			  XID sink_provider)
{
    XExtDisplayInfo	    *info = XRRFindDisplay(dpy);
    xRRSetProviderOffloadSinkReq *req;

    RRCheckExtension (dpy, info, 0);
    LockDisplay (dpy);
    GetReq (RRSetProviderOffloadSink, req);
    req->reqType = info->codes->major_opcode;
    req->randrReqType = X_RRSetProviderOffloadSink;
    req->provider = provider;
    req->sink_provider = sink_provider;
    UnlockDisplay (dpy);
    SyncHandle ();
    return 0;
}
