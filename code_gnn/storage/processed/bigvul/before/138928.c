WallpaperManager::~WallpaperManager() {
  show_user_name_on_signin_subscription_.reset();
  device_wallpaper_image_subscription_.reset();
  user_manager::UserManager::Get()->RemoveSessionStateObserver(this);
  weak_factory_.InvalidateWeakPtrs();
}
