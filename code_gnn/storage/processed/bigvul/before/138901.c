void WallpaperManager::RemovePendingWallpaperFromList(
    PendingWallpaper* pending) {
  DCHECK(loading_.size() > 0);
  for (WallpaperManager::PendingList::iterator i = loading_.begin();
       i != loading_.end();
       ++i) {
    if (i->get() == pending) {
      loading_.erase(i);
      break;
    }
  }

  if (loading_.empty()) {
    for (auto& observer : observers_)
      observer.OnPendingListEmptyForTesting();
  }
}
