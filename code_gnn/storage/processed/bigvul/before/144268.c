void UserSelectionScreen::OnUserRemoved(const AccountId& account_id) {
  if (!handler_)
    return;
  handler_->OnUserRemoved(account_id, users_.empty());
}
