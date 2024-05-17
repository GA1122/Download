void RenderWidgetHostViewAura::DidUpdateBackingStore(
    const gfx::Rect& scroll_rect,
    const gfx::Vector2d& scroll_delta,
    const std::vector<gfx::Rect>& copy_rects) {
  if (accelerated_compositing_state_changed_)
    UpdateExternalTexture();

  if (host_->is_hidden())
    return;

  gfx::Rect clip_rect;
  if (paint_canvas_) {
    SkRect sk_clip_rect;
    if (paint_canvas_->sk_canvas()->getClipBounds(&sk_clip_rect))
      clip_rect = gfx::ToEnclosingRect(gfx::SkRectToRectF(sk_clip_rect));
  }

  if (!scroll_rect.IsEmpty())
    SchedulePaintIfNotInClip(scroll_rect, clip_rect);

  for (size_t i = 0; i < copy_rects.size(); ++i) {
    gfx::Rect rect = gfx::SubtractRects(copy_rects[i], scroll_rect);
    if (rect.IsEmpty())
      continue;

    SchedulePaintIfNotInClip(rect, clip_rect);

#if defined(OS_WIN)
    gfx::Rect screen_rect = GetViewBounds();
    gfx::Rect invalid_screen_rect(rect);
    invalid_screen_rect.Offset(screen_rect.x(), screen_rect.y());
    HWND hwnd = window_->GetRootWindow()->GetAcceleratedWidget();
    PaintPluginWindowsHelper(hwnd, invalid_screen_rect);
#endif   
  }
}
