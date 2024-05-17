void VideoCaptureManager::DoStopDevice(VideoCaptureController* controller) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  DCHECK(std::find_if(
             controllers_.begin(), controllers_.end(),
             [controller](
                 const scoped_refptr<VideoCaptureController>& device_entry) {
               return device_entry.get() == controller;
             }) != controllers_.end());

  auto request_iter = device_start_request_queue_.begin();
  if (request_iter != device_start_request_queue_.end()) {
    request_iter =
        std::find_if(++request_iter, device_start_request_queue_.end(),
                     [controller](const CaptureDeviceStartRequest& request) {
                       return request.controller() == controller;
                     });
    if (request_iter != device_start_request_queue_.end()) {
      device_start_request_queue_.erase(request_iter);
      return;
    }
  }

  const media::VideoCaptureDeviceInfo* device_info =
      GetDeviceInfoById(controller->device_id());
  if (device_info != nullptr) {
    for (auto& observer : capture_observers_)
      observer.OnVideoCaptureStopped(device_info->descriptor.facing);
  }

  controller->ReleaseDeviceAsync(
      base::BindOnce([](scoped_refptr<VideoCaptureController>) {},
                     GetControllerSharedRef(controller)));
}
