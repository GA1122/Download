void RenderFrameHostImpl::OnAssociatedInterfaceRequest(
    const std::string& interface_name,
    mojo::ScopedInterfaceEndpointHandle handle) {
  ContentBrowserClient* browser_client = GetContentClient()->browser();
  if (associated_registry_->CanBindRequest(interface_name)) {
    associated_registry_->BindRequest(interface_name, std::move(handle));
  } else if (!browser_client->BindAssociatedInterfaceRequestFromFrame(
                 this, interface_name, &handle)) {
    delegate_->OnAssociatedInterfaceRequest(this, interface_name,
                                            std::move(handle));
  }
}
