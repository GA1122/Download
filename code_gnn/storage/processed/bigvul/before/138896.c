  void OnWallpaperSet() {
    DCHECK_CURRENTLY_ON(BrowserThread::UI);

    if (!BrowserThread::CurrentlyOn(BrowserThread::UI))
      return;   

    timer.Stop();   

    WallpaperManager* manager = WallpaperManager::Get();
    if (!started_load_at_.is_null()) {
      const base::TimeDelta elapsed = base::Time::Now() - started_load_at_;
      manager->SaveLastLoadTime(elapsed);
    }
    if (manager->pending_inactive_ == this) {
      manager->pending_inactive_ = NULL;
    }

    manager->RemovePendingWallpaperFromList(this);
  }
