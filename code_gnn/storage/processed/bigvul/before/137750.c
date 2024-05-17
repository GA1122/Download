void HeadlessWebContentsImpl::DidReceiveCompositorFrame() {
  TRACE_EVENT0("headless",
               "HeadlessWebContentsImpl::DidReceiveCompositorFrame");
  DCHECK(agent_host_);

  if (!first_compositor_frame_received_) {
    first_compositor_frame_received_ = true;

    base::DictionaryValue event;
    event.SetString("method",
                    "HeadlessExperimental.mainFrameReadyForScreenshots");
    event.Set("params", base::MakeUnique<base::DictionaryValue>());

    std::string json_result;
    CHECK(base::JSONWriter::Write(event, &json_result));
    for (int session_id : begin_frame_events_enabled_sessions_)
      agent_host_->SendProtocolMessageToClient(session_id, json_result);
  }

  for (const std::unique_ptr<PendingFrame>& pending_frame : pending_frames_) {
    if (!pending_frame->display_did_finish_frame)
      pending_frame->main_frame_content_updated = true;
  }
}
