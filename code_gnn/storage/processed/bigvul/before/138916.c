void WallpaperManager::SetPolicyControlledWallpaper(
    const AccountId& account_id,
    std::unique_ptr<user_manager::UserImage> user_image) {
  if (!CanGetFilesId()) {
    CallWhenCanGetFilesId(
        base::Bind(&WallpaperManager::SetPolicyControlledWallpaper,
                   weak_factory_.GetWeakPtr(), account_id,
                   base::Passed(std::move(user_image))));
    return;
  }

  const wallpaper::WallpaperFilesId wallpaper_files_id = GetFilesId(account_id);

  if (!wallpaper_files_id.is_valid())
    LOG(FATAL) << "Wallpaper flies id if invalid!";

  SetCustomWallpaper(account_id, wallpaper_files_id, "policy-controlled.jpeg",
                     wallpaper::WALLPAPER_LAYOUT_CENTER_CROPPED,
                     wallpaper::POLICY, user_image->image(),
                     user_manager::UserManager::Get()
                         ->IsUserLoggedIn()  );
}
