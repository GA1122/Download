XRenderExtRemoveDisplay (XRenderExtInfo *extinfo, Display *dpy)
{
    XRenderExtDisplayInfo *dpyinfo, *prev;

     
    _XLockMutex(_Xglobal_lock);
    prev = NULL;
    for (dpyinfo = extinfo->head; dpyinfo; dpyinfo = dpyinfo->next) {
	if (dpyinfo->display == dpy) break;
	prev = dpyinfo;
    }
    if (!dpyinfo) {
	_XUnlockMutex(_Xglobal_lock);
	return 0;		 
    }

     
    if (prev)
	prev->next = dpyinfo->next;
    else
	extinfo->head = dpyinfo->next;

    extinfo->ndisplays--;
    if (dpyinfo == extinfo->cur) extinfo->cur = NULL;   
    _XUnlockMutex(_Xglobal_lock);

    Xfree ((char *) dpyinfo);
    return 1;
}
