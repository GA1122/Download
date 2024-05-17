Time XRRTimes (Display *dpy, int screen, Time *config_timestamp)
{
  XRRScreenConfiguration *config;
  XExtDisplayInfo *info = XRRFindDisplay(dpy);
  Time ts;

  LockDisplay(dpy);
  if ((config = _XRRValidateCache(dpy, info, screen))) {
      *config_timestamp = config->config_timestamp;
      ts = config->timestamp;
      UnlockDisplay(dpy);
      return ts;
    } else {
      UnlockDisplay(dpy);
	return CurrentTime;
    }
}
