base::FilePath WallpaperManagerBase::GetCustomWallpaperDir(
    const char* sub_dir) {
  base::FilePath custom_wallpaper_dir;
  DCHECK(dir_chromeos_custom_wallpapers_path_id != -1);
  CHECK(PathService::Get(dir_chromeos_custom_wallpapers_path_id,
                         &custom_wallpaper_dir));
  return custom_wallpaper_dir.Append(sub_dir);
}
