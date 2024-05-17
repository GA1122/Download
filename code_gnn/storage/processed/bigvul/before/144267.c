void UserSelectionScreen::OnUserImageChanged(const user_manager::User& user) {
  if (!handler_)
    return;
  handler_->OnUserImageChanged(user);
}
