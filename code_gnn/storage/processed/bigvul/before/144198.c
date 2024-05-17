void LockContentsView::OnEasyUnlockIconTapped() {
  UserState* state = FindStateForUser(
      CurrentBigUserView()->GetCurrentUser()->basic_user_info->account_id);
  DCHECK(state);
  mojom::EasyUnlockIconOptionsPtr& easy_unlock_state = state->easy_unlock_state;
  DCHECK(easy_unlock_state);

  if (easy_unlock_state->hardlock_on_click) {
    AccountId user =
        CurrentBigUserView()->GetCurrentUser()->basic_user_info->account_id;
    Shell::Get()->login_screen_controller()->HardlockPod(user);
    OnClickToUnlockEnabledForUserChanged(user, false  );
  }
}
