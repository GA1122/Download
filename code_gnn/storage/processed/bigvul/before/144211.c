void LockContentsView::OnShowEasyUnlockIcon(
    const AccountId& user,
    const mojom::EasyUnlockIconOptionsPtr& icon) {
  UserState* state = FindStateForUser(user);
  if (!state)
    return;

  state->easy_unlock_state = icon->Clone();
  UpdateEasyUnlockIconForUser(user);

  LoginBigUserView* big_user =
      TryToFindBigUser(user, true  );
  if (!big_user || !big_user->auth_user())
    return;

  tooltip_bubble_->Close();
  if (icon->autoshow_tooltip) {
    tooltip_bubble_->ShowTooltip(
        icon->tooltip, big_user->auth_user()->password_view()  );
  }
}
