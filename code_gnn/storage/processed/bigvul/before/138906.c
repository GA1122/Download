  void ResetSetWallpaperImage(const gfx::ImageSkia& image,
                              const wallpaper::WallpaperInfo& info) {
    SetMode(image, info, base::FilePath(), false);
  }
