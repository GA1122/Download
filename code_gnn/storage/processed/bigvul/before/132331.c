blink::WebPermissionClient* RenderFrameImpl::permissionClient() {
  if (!permission_client_)
    permission_client_.reset(new PermissionDispatcher(GetServiceRegistry()));

  return permission_client_.get();
}
