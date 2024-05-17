void SynchronousCompositorImpl::Invalidate() {
  DCHECK(CalledOnValidThread());
  if (registered_with_client_)
    compositor_client_->PostInvalidate();
}
