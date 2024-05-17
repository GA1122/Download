bool WebContentsImpl::CheckMediaAccessPermission(const GURL& security_origin,
                                                 MediaStreamType type) {
  DCHECK(type == MEDIA_DEVICE_AUDIO_CAPTURE ||
         type == MEDIA_DEVICE_VIDEO_CAPTURE);
  return delegate_ &&
         delegate_->CheckMediaAccessPermission(this, security_origin, type);
}
