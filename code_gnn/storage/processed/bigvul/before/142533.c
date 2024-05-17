  WallpaperShownWaiter() {
    Shell::Get()->wallpaper_controller()->AddObserver(this);
  }
