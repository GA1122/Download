void WallpaperManagerBase::SaveCustomWallpaper(
    const WallpaperFilesId& wallpaper_files_id,
    const base::FilePath& original_path,
    WallpaperLayout layout,
    std::unique_ptr<gfx::ImageSkia> image) {
  base::DeleteFile(GetCustomWallpaperDir(kOriginalWallpaperSubDir)
                       .Append(wallpaper_files_id.id()),
                   true  );
  base::DeleteFile(GetCustomWallpaperDir(kSmallWallpaperSubDir)
                       .Append(wallpaper_files_id.id()),
                   true  );
  base::DeleteFile(GetCustomWallpaperDir(kLargeWallpaperSubDir)
                       .Append(wallpaper_files_id.id()),
                   true  );
  EnsureCustomWallpaperDirectories(wallpaper_files_id);
  std::string file_name = original_path.BaseName().value();
  base::FilePath small_wallpaper_path = GetCustomWallpaperPath(
      kSmallWallpaperSubDir, wallpaper_files_id, file_name);
  base::FilePath large_wallpaper_path = GetCustomWallpaperPath(
      kLargeWallpaperSubDir, wallpaper_files_id, file_name);

  ResizeAndSaveWallpaper(*image, original_path, WALLPAPER_LAYOUT_STRETCH,
                         image->width(), image->height(), nullptr);
  ResizeAndSaveWallpaper(*image, small_wallpaper_path, layout,
                         kSmallWallpaperMaxWidth, kSmallWallpaperMaxHeight,
                         nullptr);
  ResizeAndSaveWallpaper(*image, large_wallpaper_path, layout,
                         kLargeWallpaperMaxWidth, kLargeWallpaperMaxHeight,
                         nullptr);
}
