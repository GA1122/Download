void WallpaperManager::OnDeviceWallpaperPolicyChanged() {
  SetDeviceWallpaperIfApplicable(
      user_manager::UserManager::Get()->IsUserLoggedIn()
          ? user_manager::UserManager::Get()->GetActiveUser()->GetAccountId()
          : user_manager::SignInAccountId());
}
