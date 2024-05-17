 bool ResizeGripper::OnMousePressed(const views::MouseEvent& event) {
   if (!event.IsOnlyLeftMouseButton())
     return false;

  gfx::Point point(event.x(), 0);
  View::ConvertPointToScreen(this, &point);
  initial_position_ = point.x();

  return true;
}
