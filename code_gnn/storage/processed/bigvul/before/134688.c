void SynchronousCompositorExternalBeginFrameSource::SetClientReady() {
  DCHECK(CalledOnValidThread());
  SynchronousCompositorRegistry::GetInstance()->RegisterBeginFrameSource(
      routing_id_, this);
  registered_ = true;
}
