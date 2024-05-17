bool WebMediaPlayerMS::Paused() const {
  DCHECK(thread_checker_.CalledOnValidThread());
  return paused_;
}
