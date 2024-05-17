RenderFrameImpl::GetRemoteAssociatedInterfaces() {
  if (!remote_associated_interfaces_) {
    ChildThreadImpl* thread = ChildThreadImpl::current();
    if (thread) {
      mojom::AssociatedInterfaceProviderAssociatedPtr remote_interfaces;
      thread->GetRemoteRouteProvider()->GetRoute(
          routing_id_, mojo::MakeRequest(&remote_interfaces));
      remote_associated_interfaces_.reset(
          new AssociatedInterfaceProviderImpl(std::move(remote_interfaces)));
    } else {
      remote_associated_interfaces_.reset(
          new AssociatedInterfaceProviderImpl());
    }
  }
  return remote_associated_interfaces_.get();
}
