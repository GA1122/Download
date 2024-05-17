void WebMediaPlayerMS::Reload() {
  DCHECK(thread_checker_.CalledOnValidThread());
  if (web_stream_.IsNull())
    return;

  ReloadVideo();
  ReloadAudio();
}
