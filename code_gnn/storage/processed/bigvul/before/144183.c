void LockContentsView::Layout() {
  View::Layout();
  LayoutTopHeader();
  LayoutPublicSessionView();

  if (users_list_)
    users_list_->Layout();
}
