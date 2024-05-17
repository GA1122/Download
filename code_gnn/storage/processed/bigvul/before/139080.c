void AudioRendererHost::OnCloseStream(int stream_id) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  authorizations_.erase(stream_id);

  auto i = LookupIteratorById(stream_id);

  if (i == delegates_.end())
    return;

  std::swap(*i, delegates_.back());
  delegates_.pop_back();
}
