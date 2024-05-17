base::FilePath WallpaperManager::GetDeviceWallpaperFilePath() {
  return GetDeviceWallpaperDir().Append(kDeviceWallpaperFile);
}
