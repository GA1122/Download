void WallpaperManager::SetWallpaperFromImageSkia(
    const AccountId& account_id,
    const gfx::ImageSkia& image,
    wallpaper::WallpaperLayout layout,
    bool update_wallpaper) {
  DCHECK(user_manager::UserManager::Get()->IsUserLoggedIn());

  if (user_manager::UserManager::Get()->IsLoggedInAsKioskApp())
    return;
  WallpaperInfo info;
  info.layout = layout;

  wallpaper_cache_[account_id] =
      CustomWallpaperElement(base::FilePath(), image);

  if (update_wallpaper) {
    GetPendingWallpaper(last_selected_user_, false  )
        ->ResetSetWallpaperImage(image, info);
  }
}
