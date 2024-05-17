void ShelfLayoutManager::OnAppListVisibilityChanged(bool shown,
                                                    int64_t display_id) {
  if (!shelf_widget_ || !shelf_widget_->GetNativeWindow())
    return;

  if (display_.id() != display_id)
    return;

  is_app_list_visible_ = shown;
  MaybeUpdateShelfBackground(AnimationChangeType::IMMEDIATE);
}
