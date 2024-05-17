  void Reset(double delta_x, double delta_y) {
    message_loop_runner_ = new content::MessageLoopRunner;
    remaining_delta_x_ = delta_x;
    remaining_delta_y_ = delta_y;
    scroll_end_received_ = false;
  }
