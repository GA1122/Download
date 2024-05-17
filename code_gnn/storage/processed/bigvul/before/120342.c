  void SendScrollEvent(ui::EventProcessor* dispatcher,
                       float x,
                       float y,
                       int touch_id,
                       GestureEventConsumeDelegate* delegate) {
    delegate->Reset();
    ui::TouchEvent move(ui::ET_TOUCH_MOVED, gfx::PointF(x, y),
                        touch_id,
                        base::TimeDelta::FromMilliseconds(simulated_now_));
    ui::EventDispatchDetails details = dispatcher->OnEventFromSource(&move);
    ASSERT_FALSE(details.dispatcher_destroyed);
    simulated_now_++;
  }
