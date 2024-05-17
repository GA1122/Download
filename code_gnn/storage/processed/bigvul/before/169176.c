void RenderFrameHostImpl::ResourceLoadComplete(
    mojom::ResourceLoadInfoPtr resource_load_info) {
  delegate_->ResourceLoadComplete(std::move(resource_load_info));
}
