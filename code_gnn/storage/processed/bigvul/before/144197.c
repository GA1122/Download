void LockContentsView::OnEasyUnlockIconHovered() {
  LoginBigUserView* big_view = CurrentBigUserView();
  if (!big_view->auth_user())
    return;

  UserState* state =
      FindStateForUser(big_view->GetCurrentUser()->basic_user_info->account_id);
  DCHECK(state);
  mojom::EasyUnlockIconOptionsPtr& easy_unlock_state = state->easy_unlock_state;
  DCHECK(easy_unlock_state);

  if (!easy_unlock_state->tooltip.empty()) {
    tooltip_bubble_->ShowTooltip(
        easy_unlock_state->tooltip,
        big_view->auth_user()->password_view()  );
  }
}
