void LockContentsView::OnClickToUnlockEnabledForUserChanged(
    const AccountId& user,
    bool enabled) {
  LockContentsView::UserState* state = FindStateForUser(user);
  if (!state) {
    LOG(ERROR) << "Unable to find user enabling click to auth";
    return;
  }
  state->enable_tap_auth = enabled;

  LoginBigUserView* big_user =
      TryToFindBigUser(user, true  );
  if (big_user && big_user->auth_user())
    LayoutAuth(big_user, nullptr  , true  );
}
