GtkWindow* BrowserWindowGtk::GetBrowserWindowForXID(XID xid) {
  GtkWindow* window = ui::GetGtkWindowFromX11Window(xid);
  if (!GetBrowserWindowForNativeWindow(window))
    return NULL;
  return window;
}
