void UserSelectionScreen::RecordClickOnLockIcon(const AccountId& account_id) {
  EasyUnlockService* service = GetEasyUnlockServiceForUser(account_id);
  if (!service)
    return;
  service->RecordClickOnLockIcon();
}
