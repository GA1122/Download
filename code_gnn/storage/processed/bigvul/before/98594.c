void DraggedTabGtk::MoveTo(const gfx::Point& screen_point) {
  int x = screen_point.x() + mouse_tab_offset_.x() -
      ScaleValue(mouse_tab_offset_.x());
  int y = screen_point.y() + mouse_tab_offset_.y() -
      ScaleValue(mouse_tab_offset_.y());

  gtk_window_move(GTK_WINDOW(container_), x, y);
}
