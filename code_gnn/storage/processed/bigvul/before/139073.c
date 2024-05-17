void AudioRendererHost::DidValidateRenderFrame(int stream_id, bool is_valid) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  if (is_valid)
    return;

  DLOG(WARNING) << "Render frame for stream (id=" << stream_id
                << ") no longer exists.";
  OnStreamError(stream_id);
}
