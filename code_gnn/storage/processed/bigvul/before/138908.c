void WallpaperManager::SetCustomWallpaper(
    const AccountId& account_id,
    const wallpaper::WallpaperFilesId& wallpaper_files_id,
    const std::string& file,
    wallpaper::WallpaperLayout layout,
    wallpaper::WallpaperType type,
    const gfx::ImageSkia& image,
    bool update_wallpaper) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  if (user_manager::UserManager::Get()->IsLoggedInAsKioskApp())
    return;

  if (type != wallpaper::POLICY && IsPolicyControlled(account_id))
    return;

  base::FilePath wallpaper_path = GetCustomWallpaperPath(
      wallpaper::kOriginalWallpaperSubDir, wallpaper_files_id, file);

  if (image.isNull()) {
    SetDefaultWallpaperDelayed(account_id);
    return;
  }

  const user_manager::User* user =
      user_manager::UserManager::Get()->FindUser(account_id);
  CHECK(user);
  const bool is_persistent =
      !user_manager::UserManager::Get()->IsUserNonCryptohomeDataEphemeral(
          account_id) ||
      (type == wallpaper::POLICY &&
       user->GetType() == user_manager::USER_TYPE_PUBLIC_ACCOUNT);

  WallpaperInfo wallpaper_info = {
      wallpaper_path.value(),
      layout,
      type,
      base::Time::Now().LocalMidnight()
  };
  if (is_persistent) {
    image.EnsureRepsForSupportedScales();
    std::unique_ptr<gfx::ImageSkia> deep_copy(image.DeepCopy());
    scoped_refptr<base::SequencedTaskRunner> blocking_task_runner =
        base::CreateSequencedTaskRunnerWithTraits(
            {base::MayBlock(), base::TaskPriority::USER_BLOCKING,
             base::TaskShutdownBehavior::BLOCK_SHUTDOWN});
    blocking_task_runner->PostTask(
        FROM_HERE, base::BindOnce(&WallpaperManager::SaveCustomWallpaper,
                                  wallpaper_files_id,
                                  base::FilePath(wallpaper_info.location),
                                  wallpaper_info.layout,
                                  base::Passed(std::move(deep_copy))));
  }

  std::string relative_path =
      base::FilePath(wallpaper_files_id.id()).Append(file).value();
  WallpaperInfo info = {
      relative_path,
      layout,
      type,
      base::Time::Now().LocalMidnight()
  };
  SetUserWallpaperInfo(account_id, info, is_persistent);
  if (update_wallpaper) {
    GetPendingWallpaper(account_id, false)->ResetSetWallpaperImage(image, info);
  }

  wallpaper_cache_[account_id] = CustomWallpaperElement(wallpaper_path, image);
}
