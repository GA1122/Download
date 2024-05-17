void RenderFrameImpl::OnMediaPlayerActionAt(
    const gfx::PointF& location,
    const blink::WebMediaPlayerAction& action) {
  blink::WebFloatRect viewport_position(location.x(), location.y(), 0, 0);
  GetRenderWidget()->ConvertWindowToViewport(&viewport_position);
  frame_->PerformMediaPlayerAction(
      WebPoint(viewport_position.x, viewport_position.y), action);
}
