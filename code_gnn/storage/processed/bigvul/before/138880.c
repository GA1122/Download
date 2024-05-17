wallpaper::WallpaperFilesId WallpaperManager::GetFilesId(
    const AccountId& account_id) const {
  std::string stored_value;
  if (user_manager::known_user::GetStringPref(account_id, kWallpaperFilesId,
                                              &stored_value)) {
    return wallpaper::WallpaperFilesId::FromString(stored_value);
  }
  const std::string& old_id = account_id.GetUserEmail();   
  const wallpaper::WallpaperFilesId files_id = HashWallpaperFilesIdStr(old_id);
  SetKnownUserWallpaperFilesId(account_id, files_id);
  return files_id;
}
