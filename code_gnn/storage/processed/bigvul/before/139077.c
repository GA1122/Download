AudioOutputDelegate* AudioRendererHost::LookupById(int stream_id) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  auto i = LookupIteratorById(stream_id);
  return i != delegates_.end() ? i->get() : nullptr;
}
