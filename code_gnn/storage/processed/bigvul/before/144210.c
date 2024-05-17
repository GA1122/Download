void LockContentsView::OnRemoveUserWarningShown(bool is_primary) {
  Shell::Get()->login_screen_controller()->OnRemoveUserWarningShown();
}
