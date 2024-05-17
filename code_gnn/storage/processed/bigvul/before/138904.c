  void ResetSetCustomWallpaper(const wallpaper::WallpaperInfo& info,
                               const base::FilePath& wallpaper_path) {
    SetMode(gfx::ImageSkia(), info, wallpaper_path, false);
  }
