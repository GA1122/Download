GtkWindow* GetGtkWindowFromX11Window(XID xid) {
  GdkWindow* gdk_window =
      gdk_x11_window_lookup_for_display(gdk_display_get_default(), xid);
  if (!gdk_window)
    return NULL;
  GtkWindow* gtk_window = NULL;
  gdk_window_get_user_data(gdk_window,
                           reinterpret_cast<gpointer*>(&gtk_window));
  if (!gtk_window)
    return NULL;
  return gtk_window;
}
