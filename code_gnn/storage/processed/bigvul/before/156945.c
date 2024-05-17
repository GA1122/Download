blink::WebTimeRanges WebMediaPlayerMS::Buffered() const {
  DCHECK(thread_checker_.CalledOnValidThread());
  return buffered_;
}
