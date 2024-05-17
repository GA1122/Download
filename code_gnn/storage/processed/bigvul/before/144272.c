void UserSelectionScreen::SendUserList() {
  std::unique_ptr<base::ListValue> users_list =
      UpdateAndReturnUserListForWebUI();
  handler_->LoadUsers(users_to_send_, *users_list);
}
