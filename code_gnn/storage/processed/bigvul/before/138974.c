void WallpaperManagerBase::NotifyAnimationFinished() {
  for (auto& observer : observers_)
    observer.OnWallpaperAnimationFinished(last_selected_user_);
}
