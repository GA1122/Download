  base::FilePath GetCustomWallpaperPath(
      const char* sub_dir,
      const wallpaper::WallpaperFilesId& wallpaper_files_id,
      const std::string& id) {
    base::ThreadRestrictions::ScopedAllowIO allow_io;
    base::FilePath wallpaper_path =
        WallpaperManager::Get()->GetCustomWallpaperPath(sub_dir,
                                                        wallpaper_files_id, id);
    if (!base::DirectoryExists(wallpaper_path.DirName()))
      base::CreateDirectory(wallpaper_path.DirName());

    return wallpaper_path;
  }
