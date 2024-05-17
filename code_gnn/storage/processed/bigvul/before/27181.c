static void xt_client_set_info(Widget w, unsigned long flags)
{
  Atom atom_XEMBED_INFO = XInternAtom(x_display, "_XEMBED_INFO", False);

  unsigned long buffer[2];
  buffer[1] = 0;		 
  buffer[1] = flags;
  XChangeProperty(XtDisplay(w), XtWindow(w),
				  atom_XEMBED_INFO,
				  atom_XEMBED_INFO,
				  32, PropModeReplace, (unsigned char *)buffer, 2);
}