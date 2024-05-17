void BrowserView::OnExclusiveAccessUserInput() {
  if (exclusive_access_bubble_.get())
    exclusive_access_bubble_->OnUserInput();
}
