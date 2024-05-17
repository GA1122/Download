void AutomationProvider::WindowSimulateDrag(int handle,
                                            std::vector<gfx::Point> drag_path,
                                            int flags,
                                            bool press_escape_en_route,
                                            IPC::Message* reply_message) {
  gfx::NativeWindow window =
      browser_tracker_->GetResource(handle)->window()->GetNativeHandle();
  if (window && (drag_path.size() > 1)) {
    int x, y;
    gdk_window_get_position(GTK_WIDGET(window)->window, &x, &y);

    Task* next_task = new WindowDragResponseTask(this, reply_message);
    next_task = new MouseEventTask(next_task, ui_controls::UP);
    next_task = new MouseEventTask(next_task, ui_controls::UP);
    for (size_t i = drag_path.size() - 1; i > 0; --i) {
      int dest_x = drag_path[i].x() + x;
      int dest_y = drag_path[i].y() + y;
      int half_step_x = (dest_x + drag_path[i - 1].x() + x) / 2;
      int half_step_y = (dest_y + drag_path[i - 1].y() + y) / 2;

      next_task = new MouseMoveTask(next_task, dest_x, dest_y);
      next_task = new MouseMoveTask(next_task, half_step_x, half_step_y);
    }
    next_task = new MouseEventTask(next_task, ui_controls::DOWN);

    ui_controls::SendMouseMoveNotifyWhenDone(x + drag_path[0].x(),
                                             y + drag_path[0].y(),
                                             next_task);
  } else {
    AutomationMsg_WindowDrag::WriteReplyParams(reply_message, false);
    Send(reply_message);
  }
}
