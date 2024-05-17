void LauncherView::MouseDraggedOnButton(views::View* view,
                                        const views::MouseEvent& event) {
  if (!dragging_ && drag_view_ &&
      primary_axis_coordinate(abs(event.x() - drag_offset_),
                              abs(event.y() - drag_offset_)) >=
      kMinimumDragDistance) {
    PrepareForDrag(event);
  }
  if (dragging_)
    ContinueDrag(event);
}
