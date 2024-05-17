bool AudioRendererHost::IsAuthorizationStarted(int stream_id) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  return authorizations_.find(stream_id) != authorizations_.end();
}
