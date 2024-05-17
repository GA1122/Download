void HWNDMessageHandler::SchedulePaintInRect(const gfx::Rect& rect) {
  if (use_layered_buffer_) {
    invalid_rect_.Union(rect);

    if (!waiting_for_redraw_layered_window_contents_) {
      waiting_for_redraw_layered_window_contents_ = true;
      base::MessageLoop::current()->PostTask(
          FROM_HERE,
          base::Bind(&HWNDMessageHandler::RedrawLayeredWindowContents,
                     weak_factory_.GetWeakPtr()));
    }
  } else {
    RECT r = rect.ToRECT();
    InvalidateRect(hwnd(), &r, FALSE);
  }
}
