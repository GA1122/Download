Bool XRenderQueryExtension (Display *dpy, int *event_basep, int *error_basep)
{
    XRenderExtDisplayInfo *info = XRenderFindDisplay (dpy);

    if (RenderHasExtension(info)) {
	*event_basep = info->codes->first_event;
	*error_basep = info->codes->first_error;
	return True;
    } else {
	return False;
    }
}
