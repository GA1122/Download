bool WallpaperManagerBase::GetPathFromCache(const AccountId& account_id,
                                            base::FilePath* path) {
  DCHECK(thread_checker_.CalledOnValidThread());
  CustomWallpaperMap::const_iterator it = wallpaper_cache_.find(account_id);
  if (it != wallpaper_cache_.end()) {
    *path = (*it).second.first;
    return true;
  }
  return false;
}
