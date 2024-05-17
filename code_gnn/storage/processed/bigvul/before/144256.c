void UserSelectionScreen::HardLockPod(const AccountId& account_id) {
  view_->SetAuthType(account_id,
                     proximity_auth::mojom::AuthType::OFFLINE_PASSWORD,
                     base::string16());
  EasyUnlockService* service = GetEasyUnlockServiceForUser(account_id);
  if (!service)
    return;
  service->SetHardlockState(EasyUnlockScreenlockStateHandler::USER_HARDLOCK);
}
