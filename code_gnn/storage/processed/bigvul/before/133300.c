void HandleToggleDesktopBackgroundMode() {
  static int index = 0;
  DesktopBackgroundController* desktop_background_controller =
      Shell::GetInstance()->desktop_background_controller();
  switch (++index % 4) {
    case 0:
      ash::Shell::GetInstance()
          ->user_wallpaper_delegate()
          ->InitializeWallpaper();
      break;
    case 1:
      desktop_background_controller->SetWallpaperImage(
          CreateWallpaperImage(SK_ColorRED, SK_ColorBLUE),
          wallpaper::WALLPAPER_LAYOUT_STRETCH);
      break;
    case 2:
      desktop_background_controller->SetWallpaperImage(
          CreateWallpaperImage(SK_ColorBLUE, SK_ColorGREEN),
          wallpaper::WALLPAPER_LAYOUT_CENTER);
      break;
    case 3:
      desktop_background_controller->SetWallpaperImage(
          CreateWallpaperImage(SK_ColorGREEN, SK_ColorRED),
          wallpaper::WALLPAPER_LAYOUT_CENTER_CROPPED);
      break;
  }
}
