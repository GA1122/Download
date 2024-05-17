void EnsureCustomWallpaperDirectories(
    const wallpaper::WallpaperFilesId& wallpaper_files_id) {
  base::FilePath dir;
  dir = WallpaperManagerBase::GetCustomWallpaperDir(kSmallWallpaperSubDir);
  dir = dir.Append(wallpaper_files_id.id());
  if (!base::PathExists(dir))
    base::CreateDirectory(dir);
  dir = WallpaperManagerBase::GetCustomWallpaperDir(kLargeWallpaperSubDir);
  dir = dir.Append(wallpaper_files_id.id());
  if (!base::PathExists(dir))
    base::CreateDirectory(dir);
  dir = WallpaperManagerBase::GetCustomWallpaperDir(kOriginalWallpaperSubDir);
  dir = dir.Append(wallpaper_files_id.id());
  if (!base::PathExists(dir))
    base::CreateDirectory(dir);
  dir = WallpaperManagerBase::GetCustomWallpaperDir(kThumbnailWallpaperSubDir);
  dir = dir.Append(wallpaper_files_id.id());
  if (!base::PathExists(dir))
    base::CreateDirectory(dir);
}
