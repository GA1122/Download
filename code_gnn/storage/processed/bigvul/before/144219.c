void LockContentsView::SuspendImminent(
    power_manager::SuspendImminent::Reason reason) {
  LoginAuthUserView* auth_user = CurrentBigUserView()->auth_user();
  if (auth_user)
    auth_user->password_view()->Clear();
}
