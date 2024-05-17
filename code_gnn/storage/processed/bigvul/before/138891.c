void WallpaperManager::OnDeviceWallpaperDecoded(
    const AccountId& account_id,
    std::unique_ptr<user_manager::UserImage> user_image) {
  if (!user_manager::UserManager::Get()->IsUserLoggedIn()) {
    WallpaperInfo wallpaper_info = {GetDeviceWallpaperFilePath().value(),
                                    wallpaper::WALLPAPER_LAYOUT_CENTER_CROPPED,
                                    wallpaper::DEVICE,
                                    base::Time::Now().LocalMidnight()};
    GetPendingWallpaper(user_manager::SignInAccountId(), false)
        ->ResetSetWallpaperImage(user_image->image(), wallpaper_info);
  }
}
