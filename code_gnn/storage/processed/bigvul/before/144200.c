void LockContentsView::OnFocus() {
  if (primary_big_view_)
    primary_big_view_->RequestFocus();
}
