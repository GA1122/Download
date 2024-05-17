  void SendScrollEvents(ui::EventProcessor* dispatcher,
                        float x_start,
                        float y_start,
                        int dx,
                        int dy,
                        int touch_id,
                        int time_step,
                        int num_steps,
                        GestureEventConsumeDelegate* delegate) {
    int x = x_start;
    int y = y_start;

    for (int i = 0; i < num_steps; i++) {
      x += dx;
      y += dy;
      ui::TouchEvent move(ui::ET_TOUCH_MOVED, gfx::PointF(x, y),
                          touch_id,
                          base::TimeDelta::FromMilliseconds(simulated_now_));
      ui::EventDispatchDetails details = dispatcher->OnEventFromSource(&move);
      ASSERT_FALSE(details.dispatcher_destroyed);
      simulated_now_ += time_step;
    }
  }
