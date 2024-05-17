media::MediaPermission* RenderFrameImpl::GetMediaPermission() {
  if (!media_permission_dispatcher_) {
    media_permission_dispatcher_.reset(new MediaPermissionDispatcher(
        base::Bind(
            &RenderFrameImpl::GetInterface<blink::mojom::PermissionService>,
            base::Unretained(this)),
        base::Bind(&RenderFrameImpl::IsEncryptedMediaEnabled,
                   base::Unretained(this))));
  }
  return media_permission_dispatcher_.get();
}
