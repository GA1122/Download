void RenderViewHostImpl::NotifyRendererResponsive() {
  delegate_->RendererResponsive(this);
}
