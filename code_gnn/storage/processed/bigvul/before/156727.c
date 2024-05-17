  void VerifyHandlePosition() {
    ui::TouchSelectionController* touch_selection_controller =
        root_rwhv()->touch_selection_controller();

    bool handles_in_place = false;
    while (!handles_in_place) {
      gfx::PointF selection_start =
          touch_selection_controller->GetStartPosition();
      gfx::PointF selection_end = touch_selection_controller->GetEndPosition();
      gfx::RectF handle_start =
          touch_selection_controller->GetStartHandleRect();
      gfx::RectF handle_end = touch_selection_controller->GetEndHandleRect();

      if (handle_start.IsEmpty()) {
        handles_in_place = true;
      } else {
        bool has_end_handle =
            !touch_selection_controller->GetEndHandleRect().IsEmpty();
        bool start_near_y =
            std::abs(selection_start.y() - handle_start.y()) <= 3.f;
        bool start_in_x_range = selection_start.x() >= handle_start.x() &&
                                selection_start.x() <= handle_start.right();
        bool end_near_y = std::abs(selection_end.y() - handle_end.y()) <= 3.f;
        bool end_in_x_range = selection_end.x() >= handle_end.x() &&
                              selection_end.x() <= handle_end.right();
        handles_in_place = start_near_y && start_in_x_range && end_near_y &&
                           end_in_x_range && has_end_handle;
      }
      if (!handles_in_place)
        DelayBy(base::TimeDelta::FromMilliseconds(100));
    }
  }
