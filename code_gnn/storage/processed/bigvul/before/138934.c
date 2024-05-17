  int GetUpdateWallpaperCountAndReset() {
    const size_t old = update_wallpaper_count_;
    update_wallpaper_count_ = 0;
    return old;
  }
