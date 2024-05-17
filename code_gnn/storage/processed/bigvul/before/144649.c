void WebContentsImpl::RequestMediaAccessPermission(
    const MediaStreamRequest& request,
    const MediaResponseCallback& callback) {
  if (delegate_) {
    delegate_->RequestMediaAccessPermission(this, request, callback);
  } else {
    callback.Run(MediaStreamDevices(),
                 MEDIA_DEVICE_FAILED_DUE_TO_SHUTDOWN,
                 scoped_ptr<MediaStreamUI>());
  }
}
