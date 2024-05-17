XRRSetProviderOutputSource(Display *dpy, XID provider,
			   XID source_provider)
{
    XExtDisplayInfo	    *info = XRRFindDisplay(dpy);
    xRRSetProviderOutputSourceReq *req;

    RRCheckExtension (dpy, info, 0);
    LockDisplay (dpy);
    GetReq (RRSetProviderOutputSource, req);
    req->reqType = info->codes->major_opcode;
    req->randrReqType = X_RRSetProviderOutputSource;
    req->provider = provider;
    req->source_provider = source_provider;
    UnlockDisplay (dpy);
    SyncHandle ();
    return 0;
}
