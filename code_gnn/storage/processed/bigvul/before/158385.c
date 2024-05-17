void RenderWidgetHostImpl::UpdatePriority() {
  if (!destroyed_)
    process_->UpdateClientPriority(this);
}
