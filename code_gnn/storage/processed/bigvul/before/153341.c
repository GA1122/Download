void Tab::OnMouseExited(const ui::MouseEvent& event) {
  mouse_hovered_ = false;
  tab_style_->HideHover(TabStyle::HideHoverStyle::kGradual);
  UpdateForegroundColors();
  Layout();
}
