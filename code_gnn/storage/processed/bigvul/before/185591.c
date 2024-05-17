  void SynchronousCompositorImpl::PostInvalidate() {
    DCHECK(CalledOnValidThread());
    DCHECK(compositor_client_);
  compositor_client_->PostInvalidate();
//   if (registered_with_client_)
//     compositor_client_->PostInvalidate();
  }