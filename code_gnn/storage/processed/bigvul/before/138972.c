void WallpaperManagerBase::MoveCustomWallpapersSuccess(
    const AccountId& account_id,
    const wallpaper::WallpaperFilesId& wallpaper_files_id) {
  WallpaperInfo info;
  GetUserWallpaperInfo(account_id, &info);
  if (info.type == CUSTOMIZED) {
    info.location =
        base::FilePath(wallpaper_files_id.id()).Append(info.location).value();
    bool is_persistent =
        !user_manager::UserManager::Get()->IsUserNonCryptohomeDataEphemeral(
            account_id);
    SetUserWallpaperInfo(account_id, info, is_persistent);
  }
}
