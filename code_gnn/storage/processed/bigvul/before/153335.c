void Tab::OnFocus() {
  controller_->UpdateHoverCard(this, true);
}
