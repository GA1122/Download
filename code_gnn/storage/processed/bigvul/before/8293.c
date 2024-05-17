XRRSetMonitor(Display *dpy, Window window, XRRMonitorInfo *monitor)
{
    XExtDisplayInfo	    *info = XRRFindDisplay(dpy);
    xRRSetMonitorReq	    *req;

    RRSimpleCheckExtension (dpy, info);

    LockDisplay(dpy);
    GetReq (RRSetMonitor, req);
    req->reqType = info->codes->major_opcode;
    req->randrReqType = X_RRSetMonitor;
    req->length += monitor->noutput;
    req->window = window;
    req->monitor.name = monitor->name;
    req->monitor.primary = monitor->primary;
    req->monitor.automatic = False;
    req->monitor.noutput = monitor->noutput;
    req->monitor.x = monitor->x;
    req->monitor.y = monitor->y;
    req->monitor.width = monitor->width;
    req->monitor.height = monitor->height;
    req->monitor.widthInMillimeters = monitor->mwidth;
    req->monitor.heightInMillimeters = monitor->mheight;
    Data32 (dpy, monitor->outputs, monitor->noutput * 4);

    UnlockDisplay (dpy);
    SyncHandle ();
}
