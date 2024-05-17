void HWNDMessageHandler::ResetWindowRegion(bool force, bool redraw) {
  if ((window_ex_style() & WS_EX_COMPOSITED) == 0 && !custom_window_region_ &&
      (!delegate_->IsUsingCustomFrame() || !delegate_->IsWidgetWindow())) {
    if (force)
      SetWindowRgn(hwnd(), NULL, redraw);
    return;
  }

  HRGN current_rgn = CreateRectRgn(0, 0, 0, 0);
  int current_rgn_result = GetWindowRgn(hwnd(), current_rgn);

  CRect window_rect;
  GetWindowRect(hwnd(), &window_rect);
  HRGN new_region;
  if (custom_window_region_) {
    new_region = ::CreateRectRgn(0, 0, 0, 0);
    ::CombineRgn(new_region, custom_window_region_.Get(), NULL, RGN_COPY);
  } else if (IsMaximized()) {
    HMONITOR monitor = MonitorFromWindow(hwnd(), MONITOR_DEFAULTTONEAREST);
    MONITORINFO mi;
    mi.cbSize = sizeof mi;
    base::win::GetMonitorInfoWrapper(monitor, &mi);
    CRect work_rect = mi.rcWork;
    work_rect.OffsetRect(-window_rect.left, -window_rect.top);
    new_region = CreateRectRgnIndirect(&work_rect);
  } else {
    gfx::Path window_mask;
    delegate_->GetWindowMask(
        gfx::Size(window_rect.Width(), window_rect.Height()), &window_mask);
    new_region = gfx::CreateHRGNFromSkPath(window_mask);
  }

  if (current_rgn_result == ERROR || !EqualRgn(current_rgn, new_region)) {
    SetWindowRgn(hwnd(), new_region, redraw);
  } else {
    DeleteObject(new_region);
  }

  DeleteObject(current_rgn);
}
