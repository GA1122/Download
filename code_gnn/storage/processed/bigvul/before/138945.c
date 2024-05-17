void WallpaperManagerBase::CacheUserWallpaper(const AccountId& account_id) {
  DCHECK(thread_checker_.CalledOnValidThread());
  CustomWallpaperMap::iterator it = wallpaper_cache_.find(account_id);
  if (it != wallpaper_cache_.end() && !it->second.second.isNull())
    return;
  WallpaperInfo info;
  if (GetUserWallpaperInfo(account_id, &info)) {
    if (info.location.empty())
      return;

    base::FilePath wallpaper_dir;
    base::FilePath wallpaper_path;
    if (info.type == CUSTOMIZED || info.type == POLICY || info.type == DEVICE) {
      base::FilePath wallpaper_path;
      if (info.type == DEVICE) {
        wallpaper_path = GetDeviceWallpaperFilePath();
      } else {
        const char* sub_dir = GetCustomWallpaperSubdirForCurrentResolution();
        wallpaper_path = GetCustomWallpaperDir(sub_dir).Append(info.location);
      }
      wallpaper_cache_[account_id] =
          CustomWallpaperElement(wallpaper_path, gfx::ImageSkia());
      task_runner_->PostTask(
          FROM_HERE,
          base::Bind(&WallpaperManagerBase::GetCustomWallpaperInternal,
                     account_id, info, wallpaper_path,
                     false  ,
                     base::ThreadTaskRunnerHandle::Get(),
                     base::Passed(MovableOnDestroyCallbackHolder()),
                     weak_factory_.GetWeakPtr()));
      return;
    }
    LoadWallpaper(account_id, info, false  ,
                  MovableOnDestroyCallbackHolder());
  }
}
