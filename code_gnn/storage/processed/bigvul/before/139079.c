void AudioRendererHost::OnChannelClosing() {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  delegates_.clear();

  authorizations_.clear();
}
