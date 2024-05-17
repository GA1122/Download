bool WallpaperManager::IsPendingWallpaper(uint32_t image_id) {
  for (size_t i = 0; i < loading_.size(); ++i) {
    if (loading_[i]->GetImageId() == image_id) {
      return true;
    }
  }
  return false;
}
