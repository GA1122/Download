void WallpaperManager::UpdateWallpaper(bool clear_cache) {
  if (last_selected_user_.empty())
    SetDefaultWallpaperNow(user_manager::SignInAccountId());
  WallpaperManagerBase::UpdateWallpaper(clear_cache);
}
