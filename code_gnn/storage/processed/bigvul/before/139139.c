void RenderProcessHostImpl::DisableAudioDebugRecordings() {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  BrowserThread::PostTaskAndReply(
      BrowserThread::FILE, FROM_HERE, base::Bind(&base::DoNothing),
      base::Bind(&RenderProcessHostImpl::SendDisableAecDumpToRenderer,
                 weak_factory_.GetWeakPtr()));

  if (audio_input_renderer_host_) {
    BrowserThread::PostTask(
        BrowserThread::IO, FROM_HERE,
        base::Bind(&AudioInputRendererHost::DisableDebugRecording,
                   audio_input_renderer_host_));
  }
}
