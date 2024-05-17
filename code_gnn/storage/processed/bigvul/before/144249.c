EasyUnlockService* UserSelectionScreen::GetEasyUnlockServiceForUser(
    const AccountId& account_id) const {
  if (GetScreenType() == OTHER_SCREEN)
    return nullptr;

  const user_manager::User* unlock_user = nullptr;
  for (const user_manager::User* user : users_) {
    if (user->GetAccountId() == account_id) {
      unlock_user = user;
      break;
    }
  }
  if (!unlock_user)
    return nullptr;

  ProfileHelper* profile_helper = ProfileHelper::Get();
  Profile* profile = profile_helper->GetProfileByUser(unlock_user);

  DCHECK_EQ(!!profile, GetScreenType() == LOCK_SCREEN);

  if (!profile)
    profile = profile_helper->GetSigninProfile();

  return EasyUnlockService::Get(profile);
}
