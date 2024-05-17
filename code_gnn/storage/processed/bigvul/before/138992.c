void WallpaperManagerBase::TestApi::SetWallpaperCache(
    const AccountId& account_id,
    const base::FilePath& path,
    const gfx::ImageSkia& image) {
  DCHECK(!image.isNull());
  wallpaper_manager_->wallpaper_cache_[account_id] =
      CustomWallpaperElement(path, image);
}
