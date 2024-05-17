void WebContentsImpl::FocusedNodeTouched(bool editable) {
#if defined(OS_WIN)
  RenderWidgetHostView* view = GetRenderWidgetHostView();
  if (!view)
    return;
  POINT cursor_pos = {};
  ::GetCursorPos(&cursor_pos);
  float scale = GetScaleFactorForView(view);
  gfx::Point location_dips_screen =
      gfx::ConvertPointToDIP(scale, gfx::Point(cursor_pos));
  view->FocusedNodeTouched(location_dips_screen, editable);
#endif
}
