void LockScreenMediaControlsView::OnGestureEvent(ui::GestureEvent* event) {
  gfx::Point point_in_screen = event->location();
  ConvertPointToScreen(this, &point_in_screen);

  switch (event->type()) {
    case ui::ET_SCROLL_FLING_START:
    case ui::ET_GESTURE_SCROLL_BEGIN: {
      if (is_in_drag_)
        break;

      initial_drag_point_ = point_in_screen;
      is_in_drag_ = true;
      event->SetHandled();
      break;
    }
    case ui::ET_GESTURE_SCROLL_UPDATE: {
      last_fling_velocity_ = event->details().scroll_y();
      UpdateDrag(point_in_screen);
      event->SetHandled();
      break;
    }
    case ui::ET_GESTURE_END: {
      if (!is_in_drag_)
        break;

      EndDrag();
      event->SetHandled();
      break;
    }
    default:
      break;
  }
}
