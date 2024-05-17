void DesktopWindowTreeHostX11::SetAspectRatio(const gfx::SizeF& aspect_ratio) {
  XSizeHints size_hints;
  size_hints.flags = 0;
  long supplied_return;

  XGetWMNormalHints(xdisplay_, xwindow_, &size_hints, &supplied_return);
  size_hints.flags |= PAspect;
  size_hints.min_aspect.x = size_hints.max_aspect.x = aspect_ratio.width();
  size_hints.min_aspect.y = size_hints.max_aspect.y = aspect_ratio.height();
  XSetWMNormalHints(xdisplay_, xwindow_, &size_hints);
}
