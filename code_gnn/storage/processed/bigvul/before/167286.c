int ScrollableShelfView::GetActualScrollOffset() const {
  int scroll_distance = GetShelf()->IsHorizontalAlignment()
                            ? scroll_offset_.x()
                            : scroll_offset_.y();
  if (left_arrow_->GetVisible())
    scroll_distance += (kArrowButtonGroupWidth - GetAppIconEndPadding());
  return scroll_distance;
}
