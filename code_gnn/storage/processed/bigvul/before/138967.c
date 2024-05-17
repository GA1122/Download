void WallpaperManagerBase::InitInitialUserWallpaper(const AccountId& account_id,
                                                    bool is_persistent) {
  current_user_wallpaper_info_.location = "";
  current_user_wallpaper_info_.layout = WALLPAPER_LAYOUT_CENTER_CROPPED;
  current_user_wallpaper_info_.type = DEFAULT;
  current_user_wallpaper_info_.date = base::Time::Now().LocalMidnight();

  WallpaperInfo info = current_user_wallpaper_info_;
  SetUserWallpaperInfo(account_id, info, is_persistent);
}
