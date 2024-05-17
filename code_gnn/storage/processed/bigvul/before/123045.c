void RenderWidgetHostImpl::ScheduleComposite() {
  if (is_hidden_ || !is_accelerated_compositing_active_) {
      return;
  }

  if (!repaint_ack_pending_ && !resize_ack_pending_ && !view_being_painted_) {
    repaint_start_time_ = TimeTicks::Now();
    repaint_ack_pending_ = true;
    Send(new ViewMsg_Repaint(routing_id_, current_size_));
  }
}
