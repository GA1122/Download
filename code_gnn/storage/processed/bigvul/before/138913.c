bool WallpaperManager::SetDeviceWallpaperIfApplicable(
    const AccountId& account_id) {
  std::string url;
  std::string hash;
  if (ShouldSetDeviceWallpaper(account_id, &url, &hash)) {
    base::PostTaskWithTraitsAndReplyWithResult(
        FROM_HERE, {base::MayBlock()},
        base::Bind(&base::PathExists, GetDeviceWallpaperFilePath()),
        base::Bind(&WallpaperManager::OnDeviceWallpaperExists,
                   weak_factory_.GetWeakPtr(), account_id, url, hash));
    return true;
  }
  return false;
}
