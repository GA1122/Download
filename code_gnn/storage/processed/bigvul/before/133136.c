void HWNDMessageHandler::OnPaint(HDC dc) {
  PAINTSTRUCT ps;
  HDC display_dc = BeginPaint(hwnd(), &ps);
  CHECK(display_dc);

  if (!IsRectEmpty(&ps.rcPaint) &&
      !delegate_->HandlePaintAccelerated(gfx::Rect(ps.rcPaint))) {
#if defined(USE_AURA)
    delegate_->HandlePaint(NULL);
#else
    scoped_ptr<gfx::CanvasSkiaPaint> canvas(
        new gfx::CanvasSkiaPaint(hwnd(), display_dc, ps));
    delegate_->HandlePaint(canvas.get());
#endif
  }

  EndPaint(hwnd(), &ps);
}
