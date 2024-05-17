BrowserWindowGtk* BrowserWindowGtk::GetBrowserWindowForNativeWindow(
    gfx::NativeWindow window) {
  if (window) {
    return static_cast<BrowserWindowGtk*>(
        g_object_get_qdata(G_OBJECT(window), GetBrowserWindowQuarkKey()));
  }

  return NULL;
}
