void WallpaperManager::SetDefaultWallpaperDelayed(const AccountId& account_id) {
  GetPendingWallpaper(account_id, true)->ResetSetDefaultWallpaper();
}
