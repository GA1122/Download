void WallpaperManager::OnDeviceWallpaperDownloaded(const AccountId& account_id,
                                                   const std::string& hash,
                                                   bool success,
                                                   const GURL& url) {
  if (!success) {
    LOG(ERROR) << "Failed to download the device wallpaper. Fallback to "
                  "default wallpaper.";
    SetDefaultWallpaperDelayed(account_id);
    return;
  }

  base::PostTaskWithTraitsAndReplyWithResult(
      FROM_HERE, {base::MayBlock()},
      base::Bind(&CheckDeviceWallpaperMatchHash, GetDeviceWallpaperFilePath(),
                 hash),
      base::Bind(&WallpaperManager::OnCheckDeviceWallpaperMatchHash,
                 weak_factory_.GetWeakPtr(), account_id, url.spec(), hash));
}
