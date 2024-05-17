AudioRendererHost::LookupIteratorById(int stream_id) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  return std::find_if(
      delegates_.begin(), delegates_.end(),
      [stream_id](const std::unique_ptr<AudioOutputDelegate>& d) {
        return d->GetStreamId() == stream_id;
      });
}
