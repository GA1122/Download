void EglRenderingVDAClient::NotifyFlushDone() {
  if (decoder_deleted())
    return;
  SetState(CS_FLUSHED);
  --remaining_play_throughs_;
  DCHECK_GE(remaining_play_throughs_, 0);
  if (decoder_deleted())
    return;
  decoder_->Reset();
  SetState(CS_RESETTING);
}
