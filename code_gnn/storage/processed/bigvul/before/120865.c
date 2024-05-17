void OneClickSigninSyncStarter::CreateNewSignedInProfile() {
  SigninManager* signin = SigninManagerFactory::GetForProfile(profile_);
  DCHECK(!signin->GetUsernameForAuthInProgress().empty());
  DCHECK(policy_client_);
  size_t icon_index = g_browser_process->profile_manager()->
      GetProfileInfoCache().ChooseAvatarIconIndexForNewProfile();
  ProfileManager::CreateMultiProfileAsync(
      UTF8ToUTF16(signin->GetUsernameForAuthInProgress()),
      UTF8ToUTF16(ProfileInfoCache::GetDefaultAvatarIconUrl(icon_index)),
      base::Bind(&OneClickSigninSyncStarter::CompleteInitForNewProfile,
                 weak_pointer_factory_.GetWeakPtr(), desktop_type_),
      false);
}
