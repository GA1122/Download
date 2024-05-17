void PepperPlatformVideoCapture::OnDeviceOpened(int request_id,
                                                bool succeeded,
                                                const std::string& label) {
  pending_open_device_ = false;
  pending_open_device_id_ = -1;

  PepperMediaDeviceManager* const device_manager = GetMediaDeviceManager();
  succeeded = succeeded && device_manager;
  if (succeeded) {
    label_ = label;
    session_id_ = device_manager->GetSessionID(
        PP_DEVICETYPE_DEV_VIDEOCAPTURE, label);
    VideoCaptureImplManager* manager =
        RenderThreadImpl::current()->video_capture_impl_manager();
    release_device_cb_ = manager->UseDevice(session_id_);
  }

  if (handler_)
    handler_->OnInitialized(succeeded);
}
