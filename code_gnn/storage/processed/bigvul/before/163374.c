void RenderThreadImpl::OnRendererInterfaceRequest(
    mojom::RendererAssociatedRequest request) {
  DCHECK(!renderer_binding_.is_bound());
  renderer_binding_.Bind(std::move(request));
}
