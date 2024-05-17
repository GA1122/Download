  void MoveMouse(int x, int y) {
    gfx::Point screen_position(x, y);
    views::View::ConvertPointToScreen(top_container_, &screen_position);
    GetEventGenerator().MoveMouseTo(screen_position.x(), screen_position.y());

    if (controller()->top_edge_hover_timer_.IsRunning()) {
      controller()->top_edge_hover_timer_.user_task().Run();
      controller()->top_edge_hover_timer_.Stop();
    }
  }
