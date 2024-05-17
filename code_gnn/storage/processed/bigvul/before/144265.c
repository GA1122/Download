void UserSelectionScreen::OnPasswordClearTimerExpired() {
  if (handler_)
    handler_->ClearUserPodPassword();
}
