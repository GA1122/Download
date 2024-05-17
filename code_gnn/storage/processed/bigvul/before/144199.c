void LockContentsView::OnFingerprintUnlockStateChanged(
    const AccountId& account_id,
    mojom::FingerprintUnlockState state) {
  UserState* user_state = FindStateForUser(account_id);
  if (!user_state)
    return;

  user_state->fingerprint_state = state;
  LoginBigUserView* big_view =
      TryToFindBigUser(account_id, true  );
  if (!big_view || !big_view->auth_user())
    return;

  big_view->auth_user()->SetFingerprintState(user_state->fingerprint_state);
  LayoutAuth(big_view, nullptr  , true  );
}
