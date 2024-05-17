SynchronousCompositorExternalBeginFrameSource::CalledOnValidThread() const {
  return BrowserThread::CurrentlyOn(BrowserThread::UI);
}
