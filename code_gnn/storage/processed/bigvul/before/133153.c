void HWNDMessageHandler::RedrawLayeredWindowContents() {
  waiting_for_redraw_layered_window_contents_ = false;
  if (invalid_rect_.IsEmpty())
    return;

  layered_window_contents_->sk_canvas()->save(SkCanvas::kClip_SaveFlag);
  double scale = gfx::win::GetDeviceScaleFactor();
  layered_window_contents_->sk_canvas()->scale(
      SkScalar(scale),SkScalar(scale));
  layered_window_contents_->ClipRect(invalid_rect_);
  delegate_->PaintLayeredWindow(layered_window_contents_.get());
  layered_window_contents_->sk_canvas()->scale(
      SkScalar(1.0/scale),SkScalar(1.0/scale));
  layered_window_contents_->sk_canvas()->restore();

  RECT wr;
  GetWindowRect(hwnd(), &wr);
  SIZE size = {wr.right - wr.left, wr.bottom - wr.top};
  POINT position = {wr.left, wr.top};
  HDC dib_dc = skia::BeginPlatformPaint(layered_window_contents_->sk_canvas());
  POINT zero = {0, 0};
  BLENDFUNCTION blend = {AC_SRC_OVER, 0, layered_alpha_, AC_SRC_ALPHA};
  UpdateLayeredWindow(hwnd(), NULL, &position, &size, dib_dc, &zero,
                      RGB(0xFF, 0xFF, 0xFF), &blend, ULW_ALPHA);
  invalid_rect_.SetRect(0, 0, 0, 0);
  skia::EndPlatformPaint(layered_window_contents_->sk_canvas());
}
