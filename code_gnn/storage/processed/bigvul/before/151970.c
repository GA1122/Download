RenderFrameHostImpl::GetNavigationClientFromInterfaceProvider() {
  mojom::NavigationClientAssociatedPtr navigation_client_ptr;
  GetRemoteAssociatedInterfaces()->GetInterface(&navigation_client_ptr);
  return navigation_client_ptr;
}
