bool ShelfLayoutManager::CanStartFullscreenAppListDrag(
    float scroll_y_hint) const {
  if (!shelf_->IsHorizontalAlignment())
    return false;

  if (!IsVisible())
    return false;

  if (shelf_widget_->IsShowingOverflowBubble())
    return false;

  if (is_app_list_visible_)
    return false;

  if (scroll_y_hint >= 0)
    return false;

  if (IsHomeLauncherEnabledInTabletMode())
    return false;

  return true;
}
