void DesktopWindowTreeHostX11::SetCursorNative(gfx::NativeCursor cursor) {
  XDefineCursor(xdisplay_, xwindow_, cursor.platform());
}
