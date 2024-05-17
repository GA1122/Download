void LockContentsView::OnPinEnabledForUserChanged(const AccountId& user,
                                                  bool enabled) {
  LockContentsView::UserState* state = FindStateForUser(user);
  if (!state) {
    LOG(ERROR) << "Unable to find user when changing PIN state to " << enabled;
    return;
  }

  state->show_pin = enabled;

  LoginBigUserView* big_user =
      TryToFindBigUser(user, true  );
  if (big_user && big_user->auth_user())
    LayoutAuth(big_user, nullptr  , true  );
}
