XRRGetOutputPrimary(Display *dpy, Window window)
{
    XExtDisplayInfo	    *info = XRRFindDisplay(dpy);
    xRRGetOutputPrimaryReq  *req;
    xRRGetOutputPrimaryReply rep;
    int			    major_version, minor_version;

    RRCheckExtension (dpy, info, 0);

    if (!XRRQueryVersion (dpy, &major_version, &minor_version) ||
	!_XRRHasOutputPrimary (major_version, minor_version))
	return None;

    LockDisplay(dpy);
    GetReq (RRGetOutputPrimary, req);
    req->reqType	= info->codes->major_opcode;
    req->randrReqType	= X_RRGetOutputPrimary;
    req->window		= window;

    if (!_XReply (dpy, (xReply *) &rep, 0, xFalse))
	rep.output = None;

    UnlockDisplay(dpy);
    SyncHandle();

    return rep.output;
}
