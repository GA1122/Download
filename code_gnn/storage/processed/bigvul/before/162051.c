void RenderProcessHostImpl::BindInterface(
    const std::string& interface_name,
    mojo::ScopedMessagePipeHandle interface_pipe) {
  child_connection_->BindInterface(interface_name, std::move(interface_pipe));
}
