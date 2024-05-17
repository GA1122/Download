void WebPluginProxy::CreateShmPixmapFromDIB(
    TransportDIB* dib,
    const gfx::Rect& window_rect,
    XID* pixmap_out) {
  if (dib) {
    Display* display = ui::GetXDisplay();
    XID root_window = ui::GetX11RootWindow();
    XShmSegmentInfo shminfo = {0};

    if (*pixmap_out != None)
      XFreePixmap(display, *pixmap_out);

    shminfo.shmseg = dib->MapToX(display);
    *pixmap_out = XShmCreatePixmap(display, root_window,
                                   NULL, &shminfo,
                                   window_rect.width(), window_rect.height(),
                                   DefaultDepth(display,
                                                DefaultScreen(display)));
  }
}
