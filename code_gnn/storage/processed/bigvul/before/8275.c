XRenderExtFindDisplay (XRenderExtInfo *extinfo,
                       Display        *dpy)
{
    XRenderExtDisplayInfo *dpyinfo;

     
    if ((dpyinfo = extinfo->cur) && dpyinfo->display == dpy)
        return dpyinfo;

     
    _XLockMutex(_Xglobal_lock);
    for (dpyinfo = extinfo->head; dpyinfo; dpyinfo = dpyinfo->next) {
        if (dpyinfo->display == dpy) {
            extinfo->cur = dpyinfo;      
            _XUnlockMutex(_Xglobal_lock);
            return dpyinfo;
        }
    }
    _XUnlockMutex(_Xglobal_lock);

    return NULL;
}
