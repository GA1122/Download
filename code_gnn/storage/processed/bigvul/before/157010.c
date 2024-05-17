void WebMediaPlayerMS::SetVolume(double volume) {
  DVLOG(1) << __func__ << "(volume=" << volume << ")";
  DCHECK(thread_checker_.CalledOnValidThread());
  volume_ = volume;
  if (audio_renderer_.get())
    audio_renderer_->SetVolume(volume_ * volume_multiplier_);
  delegate_->DidPlayerMutedStatusChange(delegate_id_, volume == 0.0);
}
