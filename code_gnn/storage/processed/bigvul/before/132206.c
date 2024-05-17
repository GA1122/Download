media::MediaPermission* RenderFrameImpl::GetMediaPermission() {
  if (!media_permission_dispatcher_)
    media_permission_dispatcher_ = new MediaPermissionDispatcher(this);
  return media_permission_dispatcher_;
}
