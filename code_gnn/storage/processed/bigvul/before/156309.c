void MediaRecorderHandler::Stop() {
  DCHECK(main_render_thread_checker_.CalledOnValidThread());

  weak_factory_.InvalidateWeakPtrs();
  recording_ = false;
  timeslice_ = TimeDelta::FromMilliseconds(0);
  video_recorders_.clear();
  audio_recorders_.clear();
  webm_muxer_.reset();
}
