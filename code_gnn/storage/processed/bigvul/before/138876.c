WallpaperManager::GetAppropriateResolution() {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  gfx::Size size = ash::WallpaperController::GetMaxDisplaySizeInNative();
  return (size.width() > wallpaper::kSmallWallpaperMaxWidth ||
          size.height() > wallpaper::kSmallWallpaperMaxHeight)
             ? WALLPAPER_RESOLUTION_LARGE
             : WALLPAPER_RESOLUTION_SMALL;
}
