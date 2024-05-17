void WallpaperManagerBase::SetUserWallpaperNow(const AccountId& account_id) {
  ScheduleSetUserWallpaper(account_id, false);
}
