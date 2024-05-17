XRecordQueryVersion(Display *dpy, int *cmajor_return, int *cminor_return)
{
    XExtDisplayInfo *info = find_display (dpy);
    register xRecordQueryVersionReq   	*req;
    xRecordQueryVersionReply 		rep;

    XRecordCheckExtension (dpy, info, False);

    LockDisplay(dpy);
    GetReq(RecordQueryVersion, req);
    req->reqType = info->codes->major_opcode;
    req->recordReqType = X_RecordQueryVersion;
    req->majorVersion = RECORD_MAJOR_VERSION;
    req->minorVersion = RECORD_MINOR_VERSION;
    if (!_XReply(dpy,(xReply *)&rep, 0, True)) {
	UnlockDisplay(dpy);
	SyncHandle();
	return False;
    }
    UnlockDisplay(dpy);
    SyncHandle();
    *cmajor_return = rep.majorVersion;
    *cminor_return = rep.minorVersion;
    return ((rep.majorVersion == RECORD_MAJOR_VERSION) &&
	    (rep.minorVersion >= RECORD_LOWEST_MINOR_VERSION));
}
