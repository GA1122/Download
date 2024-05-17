  void SetMode(const gfx::ImageSkia& image,
               const wallpaper::WallpaperInfo& info,
               const base::FilePath& wallpaper_path,
               const bool is_default) {
    user_wallpaper_ = image;
    info_ = info;
    wallpaper_path_ = wallpaper_path;
    default_ = is_default;
  }
