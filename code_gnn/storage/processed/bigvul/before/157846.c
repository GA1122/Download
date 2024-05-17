void WebContentsImpl::RequestMediaAccessPermission(
    const MediaStreamRequest& request,
    MediaResponseCallback callback) {
  if (delegate_) {
    delegate_->RequestMediaAccessPermission(this, request, std::move(callback));
  } else {
    std::move(callback).Run(MediaStreamDevices(),
                            MEDIA_DEVICE_FAILED_DUE_TO_SHUTDOWN,
                            std::unique_ptr<MediaStreamUI>());
  }
}
