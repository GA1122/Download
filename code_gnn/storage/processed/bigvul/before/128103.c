bool SynchronousCompositorOutputSurface::CalledOnValidThread() const {
  return thread_checker_.CalledOnValidThread();
}
