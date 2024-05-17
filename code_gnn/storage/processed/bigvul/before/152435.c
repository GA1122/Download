void RenderFrameImpl::OnHostZoomClientRequest(
    mojom::HostZoomAssociatedRequest request) {
  DCHECK(!host_zoom_binding_.is_bound());
  host_zoom_binding_.Bind(std::move(request),
                          GetTaskRunner(blink::TaskType::kInternalIPC));
}
