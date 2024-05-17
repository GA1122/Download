void WallpaperManager::SetDefaultWallpaperPathsFromCommandLine(
    base::CommandLine* command_line) {
  default_small_wallpaper_file_ = command_line->GetSwitchValuePath(
      chromeos::switches::kDefaultWallpaperSmall);
  default_large_wallpaper_file_ = command_line->GetSwitchValuePath(
      chromeos::switches::kDefaultWallpaperLarge);
  guest_small_wallpaper_file_ = command_line->GetSwitchValuePath(
      chromeos::switches::kGuestWallpaperSmall);
  guest_large_wallpaper_file_ = command_line->GetSwitchValuePath(
      chromeos::switches::kGuestWallpaperLarge);
  child_small_wallpaper_file_ = command_line->GetSwitchValuePath(
      chromeos::switches::kChildWallpaperSmall);
  child_large_wallpaper_file_ = command_line->GetSwitchValuePath(
      chromeos::switches::kChildWallpaperLarge);
  default_wallpaper_image_.reset();
}
