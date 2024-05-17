base::FilePath WallpaperManager::GetDeviceWallpaperDir() {
  base::FilePath wallpaper_dir;
  if (!PathService::Get(chrome::DIR_CHROMEOS_WALLPAPERS, &wallpaper_dir)) {
    LOG(ERROR) << "Unable to get wallpaper dir.";
    return base::FilePath();
  }
  return wallpaper_dir.Append(kDeviceWallpaperDir);
}
