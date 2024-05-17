XRRSetOutputPrimary(Display *dpy, Window window, RROutput output)
{
    XExtDisplayInfo	    *info = XRRFindDisplay(dpy);
    xRRSetOutputPrimaryReq  *req;
    int			    major_version, minor_version;

    RRSimpleCheckExtension (dpy, info);

    if (!XRRQueryVersion (dpy, &major_version, &minor_version) ||
	!_XRRHasOutputPrimary (major_version, minor_version))
	return;

    LockDisplay(dpy);
    GetReq (RRSetOutputPrimary, req);
    req->reqType       = info->codes->major_opcode;
    req->randrReqType  = X_RRSetOutputPrimary;
    req->window        = window;
    req->output	       = output;

    UnlockDisplay (dpy);
    SyncHandle ();
}
