void WebContentsImpl::OnAssociatedInterfaceRequest(
    RenderFrameHost* render_frame_host,
    const std::string& interface_name,
    mojo::ScopedInterfaceEndpointHandle handle) {
  auto it = binding_sets_.find(interface_name);
  if (it != binding_sets_.end())
    it->second->OnRequestForFrame(render_frame_host, std::move(handle));
}
