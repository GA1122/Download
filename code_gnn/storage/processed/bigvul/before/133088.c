gfx::Rect HWNDMessageHandler::GetRestoredBounds() const {
  if (fullscreen_handler_->fullscreen())
    return fullscreen_handler_->GetRestoreBounds();

  gfx::Rect bounds;
  GetWindowPlacement(&bounds, NULL);
  return bounds;
}
