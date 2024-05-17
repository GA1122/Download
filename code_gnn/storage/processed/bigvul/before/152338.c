void RenderFrameImpl::GetInterface(
    const std::string& interface_name,
    mojo::ScopedMessagePipeHandle interface_pipe) {
  if (registry_.TryBindInterface(interface_name, &interface_pipe))
    return;

  for (auto& observer : observers_) {
    observer.OnInterfaceRequestForFrame(interface_name, &interface_pipe);
    if (!interface_pipe.is_valid())
      return;
  }
}
