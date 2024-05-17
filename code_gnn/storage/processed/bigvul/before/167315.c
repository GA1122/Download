bool ScrollableShelfView::ShouldApplyDisplayCentering() const {
  if (layout_strategy_ != kNotShowArrowButtons)
    return false;

  const gfx::Rect display_bounds =
      screen_util::GetDisplayBoundsWithShelf(GetWidget()->GetNativeWindow());
  const int display_size_primary = GetShelf()->PrimaryAxisValue(
      display_bounds.width(), display_bounds.height());
  StatusAreaWidget* status_widget =
      GetShelf()->shelf_widget()->status_area_widget();
  const int status_widget_size = GetShelf()->PrimaryAxisValue(
      status_widget->GetWindowBoundsInScreen().width(),
      status_widget->GetWindowBoundsInScreen().height());

  int available_space_for_screen_centering =
      display_size_primary -
      2 * (status_widget_size + ShelfConfig::Get()->app_icon_group_margin());
  return available_space_for_screen_centering >
         shelf_view_->GetSizeOfAppIcons(shelf_view_->number_of_visible_apps(),
                                        false);
}
