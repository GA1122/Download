void SynchronousCompositorImpl::SetNeedsSynchronousAnimateInput() {
  DCHECK(CalledOnValidThread());
  if (!registered_with_client_)
    return;
  need_animate_input_ = true;
  compositor_client_->PostInvalidate();
}
