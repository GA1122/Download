static IntPoint DragLocationForDHTMLDrag(const IntPoint& mouse_dragged_point,
                                         const IntPoint& drag_origin,
                                         const IntPoint& drag_image_offset,
                                         bool is_link_image) {
  const int y_offset = -drag_image_offset.Y();

  if (is_link_image) {
    return IntPoint(mouse_dragged_point.X() - drag_image_offset.X(),
                    mouse_dragged_point.Y() + y_offset);
  }

  return IntPoint(drag_origin.X() - drag_image_offset.X(),
                  drag_origin.Y() + y_offset);
}
