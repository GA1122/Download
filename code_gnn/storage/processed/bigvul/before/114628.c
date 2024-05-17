static void FlipRectVerticallyWithHeight(gfx::Rect* rect, int height) {
  rect->set_y(height - rect->bottom());
}
