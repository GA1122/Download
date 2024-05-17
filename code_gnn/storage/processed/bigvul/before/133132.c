void HWNDMessageHandler::OnNCPaint(HRGN rgn) {
  if (!delegate_->IsWidgetWindow() || !delegate_->IsUsingCustomFrame()) {
    SetMsgHandled(FALSE);
    return;
  }

  CRect window_rect;
  GetWindowRect(hwnd(), &window_rect);

  gfx::Size root_view_size = delegate_->GetRootViewSize();
  if (gfx::Size(window_rect.Width(), window_rect.Height()) != root_view_size) {
    return;
  }

  CRect dirty_region;
  if (!rgn || rgn == reinterpret_cast<HRGN>(1)) {
    dirty_region = CRect(0, 0, window_rect.Width(), window_rect.Height());
  } else {
    RECT rgn_bounding_box;
    GetRgnBox(rgn, &rgn_bounding_box);
    if (!IntersectRect(&dirty_region, &rgn_bounding_box, &window_rect))
      return;   

    OffsetRect(&dirty_region, -window_rect.left, -window_rect.top);
  }

  HDC dc = GetWindowDC(hwnd());
  ClipState clip_state;
  clip_state.x = window_rect.left;
  clip_state.y = window_rect.top;
  clip_state.parent = hwnd();
  clip_state.dc = dc;
  EnumChildWindows(hwnd(), &ClipDCToChild,
                   reinterpret_cast<LPARAM>(&clip_state));

  gfx::Rect old_paint_region = invalid_rect_;
  if (!old_paint_region.IsEmpty()) {

    CRect old_paint_region_crect = old_paint_region.ToRECT();
    CRect tmp = dirty_region;
    UnionRect(&dirty_region, &tmp, &old_paint_region_crect);
  }

  SchedulePaintInRect(gfx::Rect(dirty_region));

  if (!delegate_->HandlePaintAccelerated(gfx::Rect(dirty_region))) {
    gfx::CanvasSkiaPaint canvas(dc, true, dirty_region.left,
                                dirty_region.top, dirty_region.Width(),
                                dirty_region.Height());
    delegate_->HandlePaint(&canvas);
  }

  ReleaseDC(hwnd(), dc);
  SetMsgHandled(delegate_->IsUsingCustomFrame());
}
