void SynchronousCompositorExternalBeginFrameSource::BeginFrame(
    const cc::BeginFrameArgs& args) {
  DCHECK(CalledOnValidThread());
  CallOnBeginFrame(args);
}
