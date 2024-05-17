LRESULT HWNDMessageHandler::OnTouchEvent(UINT message,
                                         WPARAM w_param,
                                         LPARAM l_param) {
#if !defined(USE_AURA)
  SetMsgHandled(FALSE);
  return 0;
#endif
  int num_points = LOWORD(w_param);
  scoped_ptr<TOUCHINPUT[]> input(new TOUCHINPUT[num_points]);
  if (ui::GetTouchInputInfoWrapper(reinterpret_cast<HTOUCHINPUT>(l_param),
                                   num_points, input.get(),
                                   sizeof(TOUCHINPUT))) {
    TouchEvents touch_events;
    for (int i = 0; i < num_points; ++i) {
      ui::EventType touch_event_type = ui::ET_UNKNOWN;

      if (input[i].dwFlags & TOUCHEVENTF_DOWN) {
        touch_ids_.insert(input[i].dwID);
        touch_event_type = ui::ET_TOUCH_PRESSED;
      } else if (input[i].dwFlags & TOUCHEVENTF_UP) {
        touch_ids_.erase(input[i].dwID);
        touch_event_type = ui::ET_TOUCH_RELEASED;
      } else if (input[i].dwFlags & TOUCHEVENTF_MOVE) {
        touch_event_type = ui::ET_TOUCH_MOVED;
      }
      if (touch_event_type != ui::ET_UNKNOWN) {
        POINT point;
        point.x = TOUCH_COORD_TO_PIXEL(input[i].x) /
            gfx::win::GetUndocumentedDPITouchScale();
        point.y = TOUCH_COORD_TO_PIXEL(input[i].y) /
            gfx::win::GetUndocumentedDPITouchScale();

        ScreenToClient(hwnd(), &point);

        ui::TouchEvent event(
            touch_event_type,
            gfx::Point(point.x, point.y),
            id_generator_.GetGeneratedID(input[i].dwID),
            base::TimeDelta::FromMilliseconds(input[i].dwTime));
        touch_events.push_back(event);
        if (touch_event_type == ui::ET_TOUCH_RELEASED)
          id_generator_.ReleaseNumber(input[i].dwID);
      }
    }
    base::MessageLoop::current()->PostTask(
        FROM_HERE,
        base::Bind(&HWNDMessageHandler::HandleTouchEvents,
                   weak_factory_.GetWeakPtr(), touch_events));
  }
  CloseTouchInputHandle(reinterpret_cast<HTOUCHINPUT>(l_param));
  SetMsgHandled(FALSE);
  return 0;
}
