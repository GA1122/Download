float ShelfLayoutManager::GetAppListBackgroundOpacityOnShelfOpacity() {
  float shelf_opacity = shelf_widget_->GetBackgroundAlphaValue(
                            shelf_background_type_before_drag_) /
                        static_cast<float>(ShelfBackgroundAnimator::kMaxAlpha);
  const int shelf_size = ShelfConstants::shelf_size();
  if (launcher_above_shelf_bottom_amount_ < shelf_size)
    return shelf_opacity;
  float launcher_above_shelf_amount =
      std::max(0.f, launcher_above_shelf_bottom_amount_ - shelf_size);
  float coefficient =
      std::min(launcher_above_shelf_amount /
                   (app_list::AppListView::kNumOfShelfSize * shelf_size),
               1.0f);
  float app_list_view_opacity =
      is_background_blur_enabled_
          ? app_list::AppListView::kAppListOpacityWithBlur
          : app_list::AppListView::kAppListOpacity;
  return app_list_view_opacity * coefficient +
         (1 - coefficient) * shelf_opacity;
}
