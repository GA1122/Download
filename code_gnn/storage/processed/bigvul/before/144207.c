void LockContentsView::OnPublicSessionDisplayNameChanged(
    const AccountId& account_id,
    const std::string& display_name) {
  LoginUserView* user_view = TryToFindUserView(account_id);
  if (!user_view || !IsPublicAccountUser(user_view->current_user()))
    return;

  mojom::LoginUserInfoPtr user_info = user_view->current_user()->Clone();
  user_info->basic_user_info->display_name = display_name;
  user_view->UpdateForUser(user_info, false  );
}
