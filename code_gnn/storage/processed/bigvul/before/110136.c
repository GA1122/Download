int ShellWindowFrameView::NonClientHitTest(const gfx::Point& point) {
  if (frame_->IsFullscreen())
    return HTCLIENT;

#if defined(USE_ASH)
  gfx::Rect expanded_bounds = bounds();
  int outside_bounds = ui::GetDisplayLayout() == ui::LAYOUT_TOUCH ?
      kResizeOutsideBoundsSizeTouch :
      kResizeOutsideBoundsSize;
  expanded_bounds.Inset(-outside_bounds, -outside_bounds);
  if (!expanded_bounds.Contains(point))
    return HTNOWHERE;
#endif

  bool can_ever_resize = frame_->widget_delegate() ?
      frame_->widget_delegate()->CanResize() :
      false;
  int resize_border =
      frame_->IsMaximized() || frame_->IsFullscreen() ? 0 :
      kResizeInsideBoundsSize;
  int frame_component = GetHTComponentForFrame(point,
                                               resize_border,
                                               resize_border,
                                               kResizeAreaCornerSize,
                                               kResizeAreaCornerSize,
                                               can_ever_resize);
  if (frame_component != HTNOWHERE)
    return frame_component;

  int client_component = frame_->client_view()->NonClientHitTest(point);
  if (client_component != HTNOWHERE)
    return client_component;

  if (close_button_->visible() &&
      close_button_->GetMirroredBounds().Contains(point))
    return HTCLOSE;

  return HTCAPTION;
}
