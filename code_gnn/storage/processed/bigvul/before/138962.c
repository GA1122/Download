bool WallpaperManagerBase::TestApi::GetPathFromCache(
    const AccountId& account_id,
    base::FilePath* path) {
  return wallpaper_manager_->GetPathFromCache(account_id, path);
}
