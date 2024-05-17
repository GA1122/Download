void UserSelectionScreen::AttemptEasyUnlock(const AccountId& account_id) {
  EasyUnlockService* service = GetEasyUnlockServiceForUser(account_id);
  if (!service)
    return;
  service->AttemptAuth(account_id);
}
