LoginBigUserView* LockContentsView::AllocateLoginBigUserView(
    const mojom::LoginUserInfoPtr& user,
    bool is_primary) {
  LoginAuthUserView::Callbacks auth_user_callbacks;
  auth_user_callbacks.on_auth = base::BindRepeating(
      &LockContentsView::OnAuthenticate, base::Unretained(this)),
  auth_user_callbacks.on_tap = base::BindRepeating(
      &LockContentsView::SwapActiveAuthBetweenPrimaryAndSecondary,
      base::Unretained(this), is_primary),
  auth_user_callbacks.on_remove_warning_shown =
      base::BindRepeating(&LockContentsView::OnRemoveUserWarningShown,
                          base::Unretained(this), is_primary);
  auth_user_callbacks.on_remove = base::BindRepeating(
      &LockContentsView::RemoveUser, base::Unretained(this), is_primary);
  auth_user_callbacks.on_easy_unlock_icon_hovered = base::BindRepeating(
      &LockContentsView::OnEasyUnlockIconHovered, base::Unretained(this));
  auth_user_callbacks.on_easy_unlock_icon_tapped = base::BindRepeating(
      &LockContentsView::OnEasyUnlockIconTapped, base::Unretained(this));

  LoginPublicAccountUserView::Callbacks public_account_callbacks;
  public_account_callbacks.on_tap = auth_user_callbacks.on_tap;
  public_account_callbacks.on_public_account_tapped = base::BindRepeating(
      &LockContentsView::OnPublicAccountTapped, base::Unretained(this));
  return new LoginBigUserView(user, auth_user_callbacks,
                              public_account_callbacks);
}
