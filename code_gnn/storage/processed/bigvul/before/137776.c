void HeadlessWebContentsImpl::SendNeedsBeginFramesEvent(int session_id) {
  TRACE_EVENT2("headless", "HeadlessWebContentsImpl::SendNeedsBeginFramesEvent",
               "session_id", session_id, "needs_begin_frames",
               needs_external_begin_frames_);
  DCHECK(agent_host_);
  auto params = base::MakeUnique<base::DictionaryValue>();
  params->SetBoolean("needsBeginFrames", needs_external_begin_frames_);

  base::DictionaryValue event;
  event.SetString("method", "HeadlessExperimental.needsBeginFramesChanged");
  event.Set("params", std::move(params));

  std::string json_result;
  CHECK(base::JSONWriter::Write(event, &json_result));
  agent_host_->SendProtocolMessageToClient(session_id, json_result);
}
