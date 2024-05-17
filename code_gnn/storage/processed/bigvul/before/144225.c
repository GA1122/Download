LoginUserView* LockContentsView::TryToFindUserView(const AccountId& user) {
  LoginBigUserView* big_view =
      TryToFindBigUser(user, false  );
  if (big_view)
    return big_view->GetUserView();

  return users_list_->GetUserView(user);
}
