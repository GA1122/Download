 void WallpaperManager::RecordWallpaperAppType() {
  user_manager::User* user = user_manager::UserManager::Get()->GetActiveUser();
  Profile* profile = ProfileHelper::Get()->GetProfileByUser(user);

  UMA_HISTOGRAM_ENUMERATION(
      "Ash.Wallpaper.Apps",
      wallpaper_manager_util::ShouldUseAndroidWallpapersApp(profile)
          ? WALLPAPERS_APP_ANDROID
          : WALLPAPERS_PICKER_APP_CHROMEOS,
      WALLPAPERS_APPS_NUM);
}
