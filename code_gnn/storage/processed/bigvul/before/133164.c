void HWNDMessageHandler::SetBounds(const gfx::Rect& bounds_in_pixels) {
  LONG style = GetWindowLong(hwnd(), GWL_STYLE);
  if (style & WS_MAXIMIZE)
    SetWindowLong(hwnd(), GWL_STYLE, style & ~WS_MAXIMIZE);
  SetWindowPos(hwnd(), NULL, bounds_in_pixels.x(), bounds_in_pixels.y(),
               bounds_in_pixels.width(), bounds_in_pixels.height(),
               SWP_NOACTIVATE | SWP_NOZORDER);
}
