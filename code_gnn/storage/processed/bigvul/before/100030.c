void WebPluginImpl::updateVisibility(bool visible) {
  if (!window_ || !page_delegate_)
    return;

  WebPluginGeometry move;
  move.window = window_;
  move.window_rect = gfx::Rect();
  move.clip_rect = gfx::Rect();
  move.rects_valid = false;
  move.visible = visible;

  page_delegate_->DidMovePlugin(move);
}
