RenderFrameHostImpl::GetRemoteAssociatedInterfaces() {
  if (!remote_associated_interfaces_) {
    mojom::AssociatedInterfaceProviderAssociatedPtr remote_interfaces;
    IPC::ChannelProxy* channel = GetProcess()->GetChannel();
    if (channel) {
      RenderProcessHostImpl* process =
          static_cast<RenderProcessHostImpl*>(GetProcess());
      process->GetRemoteRouteProvider()->GetRoute(
          GetRoutingID(), mojo::MakeRequest(&remote_interfaces));
    } else {
      mojo::MakeIsolatedRequest(&remote_interfaces);
    }
    remote_associated_interfaces_.reset(new AssociatedInterfaceProviderImpl(
        std::move(remote_interfaces)));
  }
  return remote_associated_interfaces_.get();
}
