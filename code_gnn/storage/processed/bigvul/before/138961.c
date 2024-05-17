bool WallpaperManagerBase::GetLoggedInUserWallpaperInfo(WallpaperInfo* info) {
  DCHECK(thread_checker_.CalledOnValidThread());

  if (user_manager::UserManager::Get()->IsLoggedInAsStub()) {
    info->location = current_user_wallpaper_info_.location = "";
    info->layout = current_user_wallpaper_info_.layout =
        WALLPAPER_LAYOUT_CENTER_CROPPED;
    info->type = current_user_wallpaper_info_.type = DEFAULT;
    info->date = current_user_wallpaper_info_.date =
        base::Time::Now().LocalMidnight();
    return true;
  }

  return GetUserWallpaperInfo(
      user_manager::UserManager::Get()->GetActiveUser()->GetAccountId(), info);
}
