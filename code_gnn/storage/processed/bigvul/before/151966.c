void RenderFrameHostImpl::GetInterface(
    const std::string& interface_name,
    mojo::ScopedMessagePipeHandle interface_pipe) {
  if (!registry_ ||
      !registry_->TryBindInterface(interface_name, &interface_pipe)) {
    delegate_->OnInterfaceRequest(this, interface_name, &interface_pipe);
    if (interface_pipe.is_valid() &&
        !TryBindFrameInterface(interface_name, &interface_pipe, this)) {
      GetContentClient()->browser()->BindInterfaceRequestFromFrame(
          this, interface_name, std::move(interface_pipe));
    }
  }
}
