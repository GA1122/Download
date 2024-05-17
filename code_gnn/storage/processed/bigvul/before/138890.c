void WallpaperManager::OnCustomizedDefaultWallpaperResized(
    const GURL& wallpaper_url,
    std::unique_ptr<CustomizedWallpaperRescaledFiles> rescaled_files,
    std::unique_ptr<bool> success,
    std::unique_ptr<gfx::ImageSkia> small_wallpaper_image,
    std::unique_ptr<gfx::ImageSkia> large_wallpaper_image) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  DCHECK(rescaled_files);
  DCHECK(success.get());
  if (!*success) {
    LOG(WARNING) << "Failed to save resized customized default wallpaper";
    return;
  }
  PrefService* pref_service = g_browser_process->local_state();
  pref_service->SetString(prefs::kCustomizationDefaultWallpaperURL,
                          wallpaper_url.spec());
  SetDefaultWallpaperPath(
      rescaled_files->path_rescaled_small(), std::move(small_wallpaper_image),
      rescaled_files->path_rescaled_large(), std::move(large_wallpaper_image));
  VLOG(1) << "Customized default wallpaper applied.";
}
