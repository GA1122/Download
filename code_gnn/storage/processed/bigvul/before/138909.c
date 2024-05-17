void WallpaperManager::SetCustomizedDefaultWallpaperAfterCheck(
    const GURL& wallpaper_url,
    const base::FilePath& downloaded_file,
    std::unique_ptr<CustomizedWallpaperRescaledFiles> rescaled_files) {
  PrefService* pref_service = g_browser_process->local_state();

  std::string current_url =
      pref_service->GetString(prefs::kCustomizationDefaultWallpaperURL);
  if (current_url != wallpaper_url.spec() || !rescaled_files->AllSizesExist()) {
    DCHECK(rescaled_files->downloaded_exists());

    user_image_loader::StartWithFilePath(
        task_runner_, downloaded_file, ImageDecoder::ROBUST_JPEG_CODEC,
        0,   
        base::Bind(&WallpaperManager::OnCustomizedDefaultWallpaperDecoded,
                   weak_factory_.GetWeakPtr(), wallpaper_url,
                   base::Passed(std::move(rescaled_files))));
  } else {
    SetDefaultWallpaperPath(rescaled_files->path_rescaled_small(),
                            std::unique_ptr<gfx::ImageSkia>(),
                            rescaled_files->path_rescaled_large(),
                            std::unique_ptr<gfx::ImageSkia>());
  }
}
