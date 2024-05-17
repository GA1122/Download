void AudioRendererHost::OnStreamError(int stream_id) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  SendErrorMessage(stream_id);
  OnCloseStream(stream_id);
}
