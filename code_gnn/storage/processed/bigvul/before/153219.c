void DesktopWindowTreeHostX11::FlashFrame(bool flash_frame) {
  if (urgency_hint_set_ == flash_frame)
    return;

  gfx::XScopedPtr<XWMHints> hints(XGetWMHints(xdisplay_, xwindow_));
  if (!hints) {
    hints.reset(XAllocWMHints());
  }

  if (flash_frame)
    hints->flags |= XUrgencyHint;
  else
    hints->flags &= ~XUrgencyHint;

  XSetWMHints(xdisplay_, xwindow_, hints.get());

  urgency_hint_set_ = flash_frame;
}
