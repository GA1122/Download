void SynchronousCompositorImpl::DidActivatePendingTree() {
  if (registered_with_client_)
    compositor_client_->DidUpdateContent();
  DeliverMessages();
}
