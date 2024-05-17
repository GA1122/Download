void RenderFrameImpl::GetInterface(mojo::InterfaceRequest<Interface> request) {
  GetRemoteInterfaces()->GetInterface(std::move(request));
}
