void WebPluginDelegatePepper::SendNestedDelegateGeometryToBrowser(
    const gfx::Rect& window_rect,
    const gfx::Rect& clip_rect) {
  if (!nested_delegate_ ||
      !nested_delegate_->GetPluginWindowHandle() ||
      !render_view_) {
    return;
  }

  webkit_glue::WebPluginGeometry geom;
  geom.window = nested_delegate_->GetPluginWindowHandle();
  geom.window_rect = window_rect;
  geom.clip_rect = clip_rect;
  geom.rects_valid = true;
  geom.visible = true;
  render_view_->DidMovePlugin(geom);
}
