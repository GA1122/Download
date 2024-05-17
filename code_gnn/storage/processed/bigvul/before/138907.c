void WallpaperManager::ScheduleSetUserWallpaper(const AccountId& account_id,
                                                bool delayed) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  if (!user_manager::UserManager::IsInitialized() ||
      !g_browser_process->local_state()) {
    return;
  }

  const user_manager::User* user =
      user_manager::UserManager::Get()->FindUser(account_id);

  if (!user || user->GetType() == user_manager::USER_TYPE_KIOSK_APP ||
      user->GetType() == user_manager::USER_TYPE_ARC_KIOSK_APP) {
    return;
  }

  if (!user_manager::UserManager::Get()->IsUserLoggedIn() &&
      SetDeviceWallpaperIfApplicable(account_id))
    return;

  if ((user_manager::UserManager::Get()->IsUserNonCryptohomeDataEphemeral(
           account_id) &&
       user->HasGaiaAccount()) ||
      user->GetType() == user_manager::USER_TYPE_GUEST) {
    InitInitialUserWallpaper(account_id, false);
    GetPendingWallpaper(account_id, delayed)->ResetSetDefaultWallpaper();
    if (base::SysInfo::IsRunningOnChromeOS()) {
      LOG(ERROR)
          << "User is ephemeral or guest! Fallback to default wallpaper.";
    }
    return;
  }

  last_selected_user_ = account_id;

  WallpaperInfo info;

  if (!GetUserWallpaperInfo(account_id, &info)) {
    InitInitialUserWallpaper(account_id, true);
    GetUserWallpaperInfo(account_id, &info);
  }

  gfx::ImageSkia user_wallpaper;
  current_user_wallpaper_info_ = info;
  if (GetWallpaperFromCache(account_id, &user_wallpaper)) {
    GetPendingWallpaper(account_id, delayed)
        ->ResetSetWallpaperImage(user_wallpaper, info);
  } else {
    if (info.location.empty()) {
      GetPendingWallpaper(account_id, delayed)->ResetSetDefaultWallpaper();
      return;
    }

    if (info.type == wallpaper::CUSTOMIZED || info.type == wallpaper::POLICY ||
        info.type == wallpaper::DEVICE) {
      base::FilePath wallpaper_path;
      if (info.type != wallpaper::DEVICE) {
        const char* sub_dir = GetCustomWallpaperSubdirForCurrentResolution();
        if (info.layout == wallpaper::WALLPAPER_LAYOUT_CENTER)
          sub_dir = wallpaper::kOriginalWallpaperSubDir;
        wallpaper_path = GetCustomWallpaperDir(sub_dir);
        wallpaper_path = wallpaper_path.Append(info.location);
      } else {
        wallpaper_path = GetDeviceWallpaperFilePath();
      }

      CustomWallpaperMap::iterator it = wallpaper_cache_.find(account_id);
      if (it != wallpaper_cache_.end() && it->second.first == wallpaper_path)
        return;

      wallpaper_cache_[account_id] =
          CustomWallpaperElement(wallpaper_path, gfx::ImageSkia());
      loaded_wallpapers_for_test_++;

      GetPendingWallpaper(account_id, delayed)
          ->ResetSetCustomWallpaper(info, wallpaper_path);
      return;
    }

    GetPendingWallpaper(account_id, delayed)->ResetLoadWallpaper(info);
  }
}
