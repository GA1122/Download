void VideoCaptureManager::ResumeDevices() {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  for (auto& controller : controllers_) {
    if (controller->stream_type() != MEDIA_DEVICE_VIDEO_CAPTURE ||
        controller->IsDeviceAlive())
      continue;

    bool device_in_queue = false;
    for (auto& request : device_start_request_queue_) {
      if (request.controller() == controller.get()) {
        device_in_queue = true;
        break;
      }
    }

    if (!device_in_queue) {
      QueueStartDevice(kFakeSessionId, controller.get(),
                       controller->parameters());
    }
  }
}
