void VideoCaptureManager::ProcessDeviceStartRequestQueue() {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  auto request = device_start_request_queue_.begin();
  if (request == device_start_request_queue_.end())
    return;

  VideoCaptureController* const controller = request->controller();

  EmitLogMessage("VideoCaptureManager::ProcessDeviceStartRequestQueue", 3);
  if (controller->stream_type() == MEDIA_DEVICE_VIDEO_CAPTURE) {
    const media::VideoCaptureDeviceInfo* device_info =
        GetDeviceInfoById(controller->device_id());
    if (!device_info) {
      OnDeviceLaunchFailed(
          controller,
          media::VideoCaptureError::
              kVideoCaptureManagerProcessDeviceStartQueueDeviceInfoNotFound);
      return;
    }
    for (auto& observer : capture_observers_)
      observer.OnVideoCaptureStarted(device_info->descriptor.facing);
  }

  controller->CreateAndStartDeviceAsync(
      request->params(), static_cast<VideoCaptureDeviceLaunchObserver*>(this),
      base::BindOnce([](scoped_refptr<VideoCaptureManager>,
                        scoped_refptr<VideoCaptureController>) {},
                     scoped_refptr<VideoCaptureManager>(this),
                     GetControllerSharedRef(controller)));
}
