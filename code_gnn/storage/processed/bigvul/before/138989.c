void WallpaperManagerBase::SetPathIds(
    int dir_user_data_enum,
    int dir_chromeos_wallpapers_enum,
    int dir_chromeos_custom_wallpapers_enum) {
  dir_user_data_path_id = dir_user_data_enum;
  dir_chromeos_wallpapers_path_id = dir_chromeos_wallpapers_enum;
  dir_chromeos_custom_wallpapers_path_id = dir_chromeos_custom_wallpapers_enum;
}
