static void camera_device_status_change(
 const struct camera_module_callbacks* callbacks,
 int camera_id,
 int new_status) {
    sp<CameraService> cs = const_cast<CameraService*>(
 static_cast<const CameraService*>(callbacks));

    cs->onDeviceStatusChanged(
        camera_id,
        new_status);
}
