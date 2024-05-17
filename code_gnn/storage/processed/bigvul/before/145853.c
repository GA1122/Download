  void AttemptRevealStateChange(bool revealed, Modality modality) {
    gfx::Point event_position(0, revealed ? 0 : top_container_->height() + 100);
    switch (modality) {
      case MODALITY_MOUSE: {
        MoveMouse(event_position.x(), event_position.y());
        break;
      }
      case MODALITY_GESTURE_TAP: {
        gfx::Point screen_position = event_position;
        views::View::ConvertPointToScreen(top_container_, &screen_position);
        ui::test::EventGenerator& event_generator(GetEventGenerator());
        event_generator.MoveTouch(event_position);
        event_generator.PressTouch();
        event_generator.ReleaseTouch();
        break;
      }
      case MODALITY_GESTURE_SCROLL: {
        gfx::Point start(0, revealed ? 0 : top_container_->height() - 2);
        gfx::Vector2d scroll_delta(0, 40);
        gfx::Point end = revealed ? start + scroll_delta : start - scroll_delta;
        views::View::ConvertPointToScreen(top_container_, &start);
        views::View::ConvertPointToScreen(top_container_, &end);
        ui::test::EventGenerator& event_generator(GetEventGenerator());
        event_generator.GestureScrollSequence(
            start, end, base::TimeDelta::FromMilliseconds(30), 1);
        break;
      }
    }
  }
