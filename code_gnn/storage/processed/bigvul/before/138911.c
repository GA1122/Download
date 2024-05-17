void WallpaperManager::SetDefaultWallpaperNow(const AccountId& account_id) {
  GetPendingWallpaper(account_id, false)->ResetSetDefaultWallpaper();
}
