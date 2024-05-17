void WallpaperManagerBase::SetUserWallpaperDelayed(
    const AccountId& account_id) {
  ScheduleSetUserWallpaper(account_id, true);
}
