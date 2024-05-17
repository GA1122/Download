void RenderFrameImpl::OnSaveImageAt(int x, int y) {
  blink::WebFloatRect viewport_position(x, y, 0, 0);
  GetRenderWidget()->ConvertWindowToViewport(&viewport_position);
  frame_->SaveImageAt(WebPoint(viewport_position.x, viewport_position.y));
}
