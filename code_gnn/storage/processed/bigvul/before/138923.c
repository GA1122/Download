void WallpaperManager::StartLoad(const AccountId& account_id,
                                 const WallpaperInfo& info,
                                 bool update_wallpaper,
                                 const base::FilePath& wallpaper_path,
                                 MovableOnDestroyCallbackHolder on_finish) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  TRACE_EVENT_ASYNC_BEGIN0("ui", "LoadAndDecodeWallpaper", this);
  if (update_wallpaper) {
    wallpaper_cache_[account_id] =
        CustomWallpaperElement(wallpaper_path, gfx::ImageSkia());
  }
  user_image_loader::StartWithFilePath(
      task_runner_, wallpaper_path, ImageDecoder::ROBUST_JPEG_CODEC,
      0,   
      base::Bind(&WallpaperManager::OnWallpaperDecoded,
                 weak_factory_.GetWeakPtr(), account_id, info, update_wallpaper,
                 base::Passed(std::move(on_finish))));
}
