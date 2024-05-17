void WallpaperManagerBase::SetDefaultWallpaper(const AccountId& account_id,
                                               bool update_wallpaper) {
  RemoveUserWallpaperInfo(account_id);

  const wallpaper::WallpaperInfo info = {
      std::string(), wallpaper::WALLPAPER_LAYOUT_CENTER, DEFAULT,
      base::Time::Now().LocalMidnight()};
  const bool is_persistent =
      !user_manager::UserManager::Get()->IsUserNonCryptohomeDataEphemeral(
          account_id);
  SetUserWallpaperInfo(account_id, info, is_persistent);

  if (update_wallpaper)
    SetDefaultWallpaperNow(account_id);
}
