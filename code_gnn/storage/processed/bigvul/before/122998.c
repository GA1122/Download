void RenderWidgetHostImpl::OnMsgBeginSmoothScroll(
    int gesture_id, const ViewHostMsg_BeginSmoothScroll_Params &params) {
  if (!view_)
    return;
  active_smooth_scroll_gestures_.insert(
      std::make_pair(gesture_id,
                     view_->CreateSmoothScrollGesture(
                         params.scroll_down, params.pixels_to_scroll,
                         params.mouse_event_x, params.mouse_event_y)));

  if (!in_process_event_types_.empty())
    return;
  if (tick_active_smooth_scroll_gestures_task_posted_)
    return;
  TickActiveSmoothScrollGesture();
}
