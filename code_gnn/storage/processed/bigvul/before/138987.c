void WallpaperManagerBase::SetCustomizedDefaultWallpaper(
    const GURL& wallpaper_url,
    const base::FilePath& downloaded_file,
    const base::FilePath& resized_directory) {
  DCHECK(wallpaper_url.is_valid());
  if (!wallpaper_url.is_valid()) {
    if (!wallpaper_url.is_empty()) {
      LOG(WARNING) << "Invalid Customized Wallpaper URL '"
                   << wallpaper_url.spec() << "'";
    }
    return;
  }
  std::string downloaded_file_name = downloaded_file.BaseName().value();
  std::unique_ptr<CustomizedWallpaperRescaledFiles> rescaled_files(
      new CustomizedWallpaperRescaledFiles(
          downloaded_file, resized_directory.Append(downloaded_file_name +
                                                    kSmallWallpaperSuffix),
          resized_directory.Append(downloaded_file_name +
                                   kLargeWallpaperSuffix)));

  base::Closure check_file_exists = rescaled_files->CreateCheckerClosure();
  base::Closure on_checked_closure =
      base::Bind(&WallpaperManagerBase::SetCustomizedDefaultWallpaperAfterCheck,
                 weak_factory_.GetWeakPtr(), wallpaper_url, downloaded_file,
                 base::Passed(std::move(rescaled_files)));
  if (!task_runner_->PostTaskAndReply(FROM_HERE, check_file_exists,
                                      on_checked_closure)) {
    LOG(WARNING) << "Failed to start check CheckCustomizedWallpaperFilesExist.";
  }
}
