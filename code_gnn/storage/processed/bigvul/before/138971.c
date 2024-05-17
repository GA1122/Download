void WallpaperManagerBase::MoveCustomWallpapersOnWorker(
    const AccountId& account_id,
    const WallpaperFilesId& wallpaper_files_id,
    const scoped_refptr<base::SingleThreadTaskRunner>& reply_task_runner,
    base::WeakPtr<WallpaperManagerBase> weak_ptr) {
  const std::string& temporary_wallpaper_dir =
      account_id.GetUserEmail();   
  if (MoveCustomWallpaperDirectory(kOriginalWallpaperSubDir,
                                   temporary_wallpaper_dir,
                                   wallpaper_files_id.id())) {
    reply_task_runner->PostTask(
        FROM_HERE,
        base::Bind(&WallpaperManagerBase::MoveCustomWallpapersSuccess, weak_ptr,
                   account_id, wallpaper_files_id));
  }
  MoveCustomWallpaperDirectory(kLargeWallpaperSubDir, temporary_wallpaper_dir,
                               wallpaper_files_id.id());
  MoveCustomWallpaperDirectory(kSmallWallpaperSubDir, temporary_wallpaper_dir,
                               wallpaper_files_id.id());
  MoveCustomWallpaperDirectory(kThumbnailWallpaperSubDir,
                               temporary_wallpaper_dir,
                               wallpaper_files_id.id());
}
