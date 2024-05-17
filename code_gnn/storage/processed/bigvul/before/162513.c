void ClassicPendingScript::StreamingFinished() {
  CheckState();
  DCHECK(streamer_);   
  DCHECK(IsCurrentlyStreaming());

  if (ready_state_ == kWaitingForStreaming) {
    FinishWaitingForStreaming();
  } else if (ready_state_ == kReadyStreaming) {
    FinishReadyStreaming();
  } else {
    NOTREACHED();
  }

  DCHECK(!IsCurrentlyStreaming());
}
