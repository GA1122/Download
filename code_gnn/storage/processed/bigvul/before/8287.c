Rotation XRRRotations(Display *dpy, int screen, Rotation *current_rotation)
{
  XRRScreenConfiguration *config;
  XExtDisplayInfo *info = XRRFindDisplay(dpy);
  Rotation cr;
  LockDisplay(dpy);
  if ((config = _XRRValidateCache(dpy, info, screen))) {
    *current_rotation = config->current_rotation;
    cr = config->rotations;
    UnlockDisplay(dpy);
    return cr;
  }
  else {
    UnlockDisplay(dpy);
    *current_rotation = RR_Rotate_0;
    return 0;	 
  }
}
