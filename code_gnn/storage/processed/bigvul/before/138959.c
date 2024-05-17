base::FilePath WallpaperManagerBase::GetCustomWallpaperPath(
    const char* sub_dir,
    const wallpaper::WallpaperFilesId& wallpaper_files_id,
    const std::string& file) {
  base::FilePath custom_wallpaper_path = GetCustomWallpaperDir(sub_dir);
  return custom_wallpaper_path.Append(wallpaper_files_id.id()).Append(file);
}
