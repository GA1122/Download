void HeadlessWebContentsImpl::CreateMojoService(
    const MojoService::ServiceFactoryCallback& service_factory,
    mojo::ScopedMessagePipeHandle handle) {
  service_factory.Run(this, std::move(handle));
}
