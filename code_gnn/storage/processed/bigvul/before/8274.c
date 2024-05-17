XRenderExtAddDisplay (XRenderExtInfo *extinfo,
                      Display        *dpy,
                      char           *ext_name)
{
    XRenderExtDisplayInfo *dpyinfo;

    dpyinfo = (XRenderExtDisplayInfo *) Xmalloc (sizeof (XRenderExtDisplayInfo));
    if (!dpyinfo) return NULL;
    dpyinfo->display = dpy;
    dpyinfo->info = NULL;

    if (XRenderHasDepths (dpy))
	dpyinfo->codes = XInitExtension (dpy, ext_name);
    else
	dpyinfo->codes = NULL;

     
    if (dpyinfo->codes) {
        XESetCloseDisplay (dpy, dpyinfo->codes->extension,
                           XRenderCloseDisplay);
    } else {
	 
	XExtCodes *codes = XAddExtension(dpy);
	if (!codes) {
	    XFree(dpyinfo);
	    return NULL;
	}
        XESetCloseDisplay (dpy, codes->extension, XRenderCloseDisplay);
    }

     
    _XLockMutex(_Xglobal_lock);
    dpyinfo->next = extinfo->head;
    extinfo->head = dpyinfo;
    extinfo->cur = dpyinfo;
    extinfo->ndisplays++;
    _XUnlockMutex(_Xglobal_lock);
    return dpyinfo;
}
