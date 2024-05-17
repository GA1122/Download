RenderFrameImpl::GetRemoteAssociatedInterfaces() {
  if (!remote_associated_interfaces_) {
    ChildThreadImpl* thread = ChildThreadImpl::current();
    if (thread) {
      blink::mojom::AssociatedInterfaceProviderAssociatedPtr remote_interfaces;
      thread->GetRemoteRouteProvider()->GetRoute(
          routing_id_, mojo::MakeRequest(&remote_interfaces));
      remote_associated_interfaces_ =
          std::make_unique<blink::AssociatedInterfaceProvider>(
              std::move(remote_interfaces),
              GetTaskRunner(blink::TaskType::kInternalIPC));
    } else {
      remote_associated_interfaces_ =
          std::make_unique<blink::AssociatedInterfaceProvider>(
              GetTaskRunner(blink::TaskType::kInternalIPC));
    }
  }
  return remote_associated_interfaces_.get();
}
