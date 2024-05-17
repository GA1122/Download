bool WebContentsImpl::CheckMediaAccessPermission(
    RenderFrameHost* render_frame_host,
    const url::Origin& security_origin,
    MediaStreamType type) {
  DCHECK(type == MEDIA_DEVICE_AUDIO_CAPTURE ||
         type == MEDIA_DEVICE_VIDEO_CAPTURE);
  return delegate_ && delegate_->CheckMediaAccessPermission(
                          render_frame_host, security_origin.GetURL(), type);
}
