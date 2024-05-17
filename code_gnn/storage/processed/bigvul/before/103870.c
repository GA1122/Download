WebKit::WebDeviceOrientationClient* RenderView::deviceOrientationClient() {
  if (!device_orientation_dispatcher_)
    device_orientation_dispatcher_ = new DeviceOrientationDispatcher(this);
  return device_orientation_dispatcher_;
}
