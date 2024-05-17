XRRDeleteMonitor(Display *dpy, Window window, Atom name)
{
    XExtDisplayInfo	    *info = XRRFindDisplay(dpy);
    xRRDeleteMonitorReq	    *req;

    RRSimpleCheckExtension (dpy, info);

    LockDisplay(dpy);
    GetReq (RRDeleteMonitor, req);
    req->reqType = info->codes->major_opcode;
    req->randrReqType = X_RRDeleteMonitor;
    req->window = window;
    req->name = name;
    UnlockDisplay (dpy);
    SyncHandle ();
}
