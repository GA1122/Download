void LockContentsView::OnForceOnlineSignInForUser(const AccountId& user) {
  LockContentsView::UserState* state = FindStateForUser(user);
  if (!state) {
    LOG(ERROR) << "Unable to find user forcing online sign in";
    return;
  }
  state->force_online_sign_in = true;

  LoginBigUserView* big_user =
      TryToFindBigUser(user, true  );
  if (big_user && big_user->auth_user())
    LayoutAuth(big_user, nullptr  , true  );
}
