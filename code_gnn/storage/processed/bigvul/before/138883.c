WallpaperManager::PendingWallpaper* WallpaperManager::GetPendingWallpaper(
    const AccountId& account_id,
    bool delayed) {
  if (!pending_inactive_) {
    loading_.push_back(new WallpaperManager::PendingWallpaper(
        (delayed ? GetWallpaperLoadDelay()
                 : base::TimeDelta::FromMilliseconds(0)),
        account_id));
    pending_inactive_ = loading_.back().get();
  }
  return pending_inactive_;
}
