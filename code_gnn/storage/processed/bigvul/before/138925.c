void WallpaperManager::UserChangedChildStatus(user_manager::User* user) {
  SetUserWallpaperNow(user->GetAccountId());
}
