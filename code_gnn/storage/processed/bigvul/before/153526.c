void GM2TabStyle::HideHover(HideHoverStyle style) {
  if (hover_controller_)
    hover_controller_->Hide(style);
}
