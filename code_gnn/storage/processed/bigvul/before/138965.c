bool WallpaperManagerBase::GetWallpaperFromCache(const AccountId& account_id,
                                                 gfx::ImageSkia* image) {
  DCHECK(thread_checker_.CalledOnValidThread());
  CustomWallpaperMap::const_iterator it = wallpaper_cache_.find(account_id);
  if (it != wallpaper_cache_.end() && !(*it).second.second.isNull()) {
    *image = (*it).second.second;
    return true;
  }
  return false;
}
