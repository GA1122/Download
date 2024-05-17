void RenderViewHostImpl::OnRunModal(int opener_id, IPC::Message* reply_msg) {
  DCHECK(!run_modal_reply_msg_);
  run_modal_reply_msg_ = reply_msg;
  run_modal_opener_id_ = opener_id;

  RecordAction(UserMetricsAction("ShowModalDialog"));

  RenderViewHostImpl* opener =
      RenderViewHostImpl::FromID(GetProcess()->GetID(), run_modal_opener_id_);
  if (opener) {
    opener->StopHangMonitorTimeout();
    opener->decrement_in_flight_event_count();
  }

}
