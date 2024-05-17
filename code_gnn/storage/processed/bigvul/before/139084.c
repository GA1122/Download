void AudioRendererHost::OnPlayStream(int stream_id) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  AudioOutputDelegate* delegate = LookupById(stream_id);
  if (!delegate) {
    SendErrorMessage(stream_id);
    return;
  }

  delegate->OnPlayStream();
}
