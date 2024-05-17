void ClientControlledShellSurface::SetFrameButtons(
    uint32_t visible_button_mask,
    uint32_t enabled_button_mask) {
  if (frame_visible_button_mask_ == visible_button_mask &&
      frame_enabled_button_mask_ == enabled_button_mask) {
    return;
  }
  frame_visible_button_mask_ = visible_button_mask;
  frame_enabled_button_mask_ = enabled_button_mask;

  if (widget_)
    UpdateCaptionButtonModel();
}
