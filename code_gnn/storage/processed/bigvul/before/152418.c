void RenderFrameImpl::OnAssociatedInterfaceRequest(
    const std::string& interface_name,
    mojo::ScopedInterfaceEndpointHandle handle) {
  if (!associated_interfaces_.TryBindInterface(interface_name, &handle)) {
    for (auto& observer : observers_) {
      if (observer.OnAssociatedInterfaceRequestForFrame(interface_name,
                                                        &handle)) {
        return;
      }
    }
  }
}
