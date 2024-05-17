void Tab::OnMouseMoved(const ui::MouseEvent& event) {
  tab_style_->SetHoverLocation(event.location());
  controller_->OnMouseEventInTab(this, event);
}
