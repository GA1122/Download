void LayerTreeHostImpl::RequestImplSideInvalidation() {
  client_->NeedsImplSideInvalidation();
}
