void Tab::OnMouseEntered(const ui::MouseEvent& event) {
  mouse_hovered_ = true;
  tab_style_->ShowHover(TabStyle::ShowHoverStyle::kSubtle);
  UpdateForegroundColors();
  Layout();
  controller_->UpdateHoverCard(this, true);
}
