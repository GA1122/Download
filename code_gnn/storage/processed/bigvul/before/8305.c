XFixesCreateRegionFromPicture (Display *dpy, XID picture)
{
    XFixesExtDisplayInfo		*info = XFixesFindDisplay (dpy);
    xXFixesCreateRegionFromPictureReq	*req;
    XserverRegion			region;

    XFixesCheckExtension (dpy, info, 0);
    LockDisplay (dpy);
    GetReq (XFixesCreateRegionFromPicture, req);
    req->reqType = info->codes->major_opcode;
    req->xfixesReqType = X_XFixesCreateRegionFromPicture;
    region = req->region = XAllocID (dpy);
    req->picture = picture;
    UnlockDisplay (dpy);
    SyncHandle();
    return region;
}
