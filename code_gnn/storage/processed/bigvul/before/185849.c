  void WallpaperManager::StartLoadAndSetDefaultWallpaper(
      const base::FilePath& path,
      const wallpaper::WallpaperLayout layout,
//     bool update_wallpaper,
      MovableOnDestroyCallbackHolder on_finish,
      std::unique_ptr<user_manager::UserImage>* result_out) {
    user_image_loader::StartWithFilePath(
        task_runner_, path, ImageDecoder::ROBUST_JPEG_CODEC,
        0,   
        base::Bind(&WallpaperManager::OnDefaultWallpaperDecoded,
                 weak_factory_.GetWeakPtr(), path, layout,
//                  weak_factory_.GetWeakPtr(), path, layout, update_wallpaper,
                   base::Unretained(result_out),
                   base::Passed(std::move(on_finish))));
  }