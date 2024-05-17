void LockContentsView::OnPublicSessionKeyboardLayoutsChanged(
    const AccountId& account_id,
    const std::string& locale,
    const std::vector<mojom::InputMethodItemPtr>& keyboard_layouts) {
  if (expanded_view_->visible() &&
      expanded_view_->current_user()->basic_user_info->account_id ==
          account_id) {
    mojom::LoginUserInfoPtr user_info = expanded_view_->current_user()->Clone();
    user_info->public_account_info->default_locale = locale;
    user_info->public_account_info->keyboard_layouts =
        mojo::Clone(keyboard_layouts);
    expanded_view_->UpdateForUser(user_info);
  }

  LoginUserView* user_view = TryToFindUserView(account_id);
  if (!user_view || !IsPublicAccountUser(user_view->current_user())) {
    LOG(ERROR) << "Unable to find public account user.";
    return;
  }

  mojom::LoginUserInfoPtr user_info = user_view->current_user()->Clone();
  if (user_info->public_account_info->default_locale != locale)
    return;

  user_info->public_account_info->keyboard_layouts =
      mojo::Clone(keyboard_layouts);
  user_view->UpdateForUser(user_info, false  );
}
