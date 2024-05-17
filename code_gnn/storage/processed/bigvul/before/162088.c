const service_manager::Identity& RenderProcessHostImpl::GetChildIdentity()
    const {
  return child_connection_->child_identity();
}
