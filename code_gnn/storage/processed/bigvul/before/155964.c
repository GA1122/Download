void DiceTurnSyncOnHelper::CreateNewSignedInProfile() {
  size_t icon_index = g_browser_process->profile_manager()
                          ->GetProfileAttributesStorage()
                          .ChooseAvatarIconIndexForNewProfile();
  ProfileManager::CreateMultiProfileAsync(
      base::UTF8ToUTF16(account_info_.email),
      profiles::GetDefaultAvatarIconUrl(icon_index),
      base::BindRepeating(&DiceTurnSyncOnHelper::CompleteInitForNewProfile,
                          weak_pointer_factory_.GetWeakPtr()));
}
