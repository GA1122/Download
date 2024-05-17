void LockContentsView::LayoutAuth(LoginBigUserView* to_update,
                                  LoginBigUserView* opt_to_hide,
                                  bool animate) {
  DCHECK(to_update);
  UpdateAuthForAuthUser(to_update->auth_user(),
                        opt_to_hide ? opt_to_hide->auth_user() : nullptr,
                        animate);
  UpdateAuthForPublicAccount(
      to_update->public_account(),
      opt_to_hide ? opt_to_hide->public_account() : nullptr, animate);
}
