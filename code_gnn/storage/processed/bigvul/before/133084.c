bool HWNDMessageHandler::GetClientAreaInsets(gfx::Insets* insets) const {
  if (delegate_->GetClientAreaInsets(insets))
    return true;
  DCHECK(insets->empty());

  if (!delegate_->IsWidgetWindow() ||
      (!delegate_->IsUsingCustomFrame() && !remove_standard_frame_)) {
    return false;
  }

  if (IsMaximized()) {
    int border_thickness = GetSystemMetrics(SM_CXSIZEFRAME);
    if (remove_standard_frame_)
      border_thickness -= 1;
    *insets = gfx::Insets(
        border_thickness, border_thickness, border_thickness, border_thickness);
    return true;
  }

  *insets = gfx::Insets();
  return true;
}
