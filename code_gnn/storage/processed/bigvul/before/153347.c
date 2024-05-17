void Tab::OnViewFocused(views::View* observed_view) {
  controller_->UpdateHoverCard(this, true);
}
