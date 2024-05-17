bool ClassicPendingScript::StartStreamingIfPossible(
    ScriptStreamer::Type streamer_type,
    base::OnceClosure done) {
  if (IsCurrentlyStreaming())
    return false;

  if (ready_state_ != kWaitingForResource && ready_state_ != kReady)
    return false;

  Document* document = &GetElement()->GetDocument();
  if (!document || !document->GetFrame())
    return false;

  ScriptState* script_state = ToScriptStateForMainWorld(document->GetFrame());
  if (!script_state)
    return false;

  if (streamer_ && streamer_->StreamingSuppressed() &&
      streamer_->IsFinished()) {
    DCHECK_EQ(ready_state_, kReady);
    DCHECK(!streamer_done_);
    DCHECK(!IsCurrentlyStreaming());
    streamer_.Clear();
  }

  if (streamer_)
    return false;

  DCHECK(!IsCurrentlyStreaming());

  auto task_type = streamer_type == ScriptStreamer::kParsingBlocking
                       ? TaskType::kNetworking
                       : TaskType::kNetworkingControl;

  DCHECK(!streamer_);
  DCHECK(!IsCurrentlyStreaming());
  DCHECK(!streamer_done_);
  ScriptStreamer::StartStreaming(
      this, streamer_type, document->GetFrame()->GetSettings(), script_state,
      document->GetTaskRunner(task_type));
  bool success = streamer_ && !streamer_->IsStreamingFinished();

  DCHECK_EQ(success, IsCurrentlyStreaming());
  if (success)
    streamer_done_ = std::move(done);
  return success;
}
