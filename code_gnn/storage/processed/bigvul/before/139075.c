void AudioRendererHost::GetOutputControllers(
    const RenderProcessHost::GetAudioOutputControllersCallback&
        callback) const {
  BrowserThread::PostTaskAndReplyWithResult(
      BrowserThread::IO, FROM_HERE,
      base::Bind(&AudioRendererHost::DoGetOutputControllers, this), callback);
}
