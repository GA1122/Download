  PendingWallpaper(const base::TimeDelta delay, const AccountId& account_id)
      : account_id_(account_id),
        default_(false),
        on_finish_(new MovableOnDestroyCallback(
            base::Bind(&WallpaperManager::PendingWallpaper::OnWallpaperSet,
                       this))) {
    timer.Start(
        FROM_HERE,
        delay,
        base::Bind(&WallpaperManager::PendingWallpaper::ProcessRequest, this));
  }
