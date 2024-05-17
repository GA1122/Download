Atom GetAtom(const char* name) {
#if defined(TOOLKIT_GTK)
  return gdk_x11_get_xatom_by_name_for_display(
      gdk_display_get_default(), name);
#else
  return XInternAtom(GetXDisplay(), name, false);
#endif
}
