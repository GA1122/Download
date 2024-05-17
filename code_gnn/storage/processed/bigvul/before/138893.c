void WallpaperManager::OnDeviceWallpaperExists(const AccountId& account_id,
                                               const std::string& url,
                                               const std::string& hash,
                                               bool exist) {
  if (exist) {
    base::PostTaskWithTraitsAndReplyWithResult(
        FROM_HERE, {base::MayBlock()},
        base::Bind(&CheckDeviceWallpaperMatchHash, GetDeviceWallpaperFilePath(),
                   hash),
        base::Bind(&WallpaperManager::OnCheckDeviceWallpaperMatchHash,
                   weak_factory_.GetWeakPtr(), account_id, url, hash));
  } else {
    GURL device_wallpaper_url(url);
    device_wallpaper_downloader_.reset(new CustomizationWallpaperDownloader(
        g_browser_process->system_request_context(), device_wallpaper_url,
        GetDeviceWallpaperDir(), GetDeviceWallpaperFilePath(),
        base::Bind(&WallpaperManager::OnDeviceWallpaperDownloaded,
                   weak_factory_.GetWeakPtr(), account_id, hash)));
    device_wallpaper_downloader_->Start();
  }
}
