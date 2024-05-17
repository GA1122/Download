void RenderThreadImpl::SetResourceDispatcherDelegate(
    content::ResourceDispatcherDelegate* delegate) {
  resource_dispatcher()->set_delegate(delegate);
}
