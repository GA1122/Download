ShelfBackgroundAnimator::ShelfBackgroundAnimator(
    ShelfBackgroundType background_type,
    Shelf* shelf,
    WallpaperController* wallpaper_controller)
    : shelf_(shelf), wallpaper_controller_(wallpaper_controller) {
  if (wallpaper_controller_)
    wallpaper_controller_->AddObserver(this);
  if (shelf_)
    shelf_->AddObserver(this);

  AnimateBackground(background_type, AnimationChangeType::IMMEDIATE);
}
