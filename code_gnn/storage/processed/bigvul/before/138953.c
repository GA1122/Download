void WallpaperManagerBase::DeleteUserWallpapers(
    const AccountId& account_id,
    const std::string& path_to_file) {
  std::vector<base::FilePath> file_to_remove;
  base::FilePath wallpaper_path = GetCustomWallpaperDir(kSmallWallpaperSubDir);
  file_to_remove.push_back(wallpaper_path.Append(account_id.GetUserEmail()));
  wallpaper_path = wallpaper_path.Append(path_to_file).DirName();
  file_to_remove.push_back(wallpaper_path);

  wallpaper_path = GetCustomWallpaperDir(kLargeWallpaperSubDir);
  file_to_remove.push_back(wallpaper_path.Append(account_id.GetUserEmail()));
  wallpaper_path = wallpaper_path.Append(path_to_file);
  file_to_remove.push_back(wallpaper_path);

  wallpaper_path = GetCustomWallpaperDir(kThumbnailWallpaperSubDir);
  file_to_remove.push_back(wallpaper_path.Append(account_id.GetUserEmail()));
  wallpaper_path = wallpaper_path.Append(path_to_file);
  file_to_remove.push_back(wallpaper_path);

  wallpaper_path = GetCustomWallpaperDir(kOriginalWallpaperSubDir);
  file_to_remove.push_back(wallpaper_path.Append(account_id.GetUserEmail()));
  wallpaper_path = wallpaper_path.Append(path_to_file);
  file_to_remove.push_back(wallpaper_path);

  base::PostTaskWithTraits(FROM_HERE,
                           {base::MayBlock(), base::TaskPriority::BACKGROUND,
                            base::TaskShutdownBehavior::CONTINUE_ON_SHUTDOWN},
                           base::Bind(&DeleteWallpaperInList, file_to_remove));
}
