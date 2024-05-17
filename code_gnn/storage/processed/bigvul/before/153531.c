void GM2TabStyle::ShowHover(ShowHoverStyle style) {
  if (!hover_controller_)
    return;

  if (style == ShowHoverStyle::kSubtle) {
    hover_controller_->SetSubtleOpacityScale(
        tab_->controller()->GetHoverOpacityForRadialHighlight());
  }
  hover_controller_->Show(style);
}
