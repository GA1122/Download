void SetKnownUserWallpaperFilesId(
    const AccountId& account_id,
    const wallpaper::WallpaperFilesId& wallpaper_files_id) {
  user_manager::known_user::SetStringPref(account_id, kWallpaperFilesId,
                                          wallpaper_files_id.id());
}
