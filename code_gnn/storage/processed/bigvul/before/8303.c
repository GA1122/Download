XFixesCreateRegionFromBitmap (Display *dpy, Pixmap bitmap)
{
    XFixesExtDisplayInfo		*info = XFixesFindDisplay (dpy);
    xXFixesCreateRegionFromBitmapReq	*req;
    XserverRegion			region;

    XFixesCheckExtension (dpy, info, 0);
    LockDisplay (dpy);
    GetReq (XFixesCreateRegionFromBitmap, req);
    req->reqType = info->codes->major_opcode;
    req->xfixesReqType = X_XFixesCreateRegionFromBitmap;
    region = req->region = XAllocID (dpy);
    req->bitmap = bitmap;
    UnlockDisplay (dpy);
    SyncHandle();
    return region;
}
