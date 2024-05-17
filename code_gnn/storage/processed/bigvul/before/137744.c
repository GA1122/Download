void HeadlessWebContentsImpl::CreateTabSocketMojoService(
    mojo::ScopedMessagePipeHandle handle) {
  headless_tab_socket_->CreateMojoService(TabSocketRequest(std::move(handle)));
}
