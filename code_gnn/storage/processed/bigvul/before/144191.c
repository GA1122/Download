void LockContentsView::OnAuthEnabledForUserChanged(
    const AccountId& user,
    bool enabled,
    const base::Optional<base::Time>& auth_reenabled_time) {
  LockContentsView::UserState* state = FindStateForUser(user);
  if (!state) {
    LOG(ERROR) << "Unable to find user when changing auth enabled state to "
               << enabled;
    return;
  }

  DCHECK(enabled || auth_reenabled_time);
  state->disable_auth = !enabled;
  if (state->disable_auth)
    DisableLockScreenNote();

  LoginBigUserView* big_user =
      TryToFindBigUser(user, true  );
  if (big_user && big_user->auth_user()) {
    LayoutAuth(big_user, nullptr  , true  );
    if (auth_reenabled_time)
      big_user->auth_user()->SetAuthReenabledTime(auth_reenabled_time.value());
  }
}
