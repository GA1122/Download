void Camera3Device::sProcessCaptureResult(const camera3_callback_ops *cb,
 const camera3_capture_result *result) {
 Camera3Device *d =
 const_cast<Camera3Device*>(static_cast<const Camera3Device*>(cb));

    d->processCaptureResult(result);
}
