void WallpaperManager::Open() {
  if (wallpaper_manager_util::ShouldUseAndroidWallpapersApp(
          ProfileHelper::Get()->GetProfileByUser(
              user_manager::UserManager::Get()->GetActiveUser())) &&
      !ash_util::IsRunningInMash()) {
    activation_client_observer_.Add(ash::Shell::Get()->activation_client());
  }
  wallpaper_manager_util::OpenWallpaperManager();
}
