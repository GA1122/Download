PepperPlatformVideoCapture::~PepperPlatformVideoCapture() {
  DCHECK(stop_capture_cb_.is_null());
  DCHECK(release_device_cb_.is_null());
  DCHECK(label_.empty());
  DCHECK(!pending_open_device_);
}
