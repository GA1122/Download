void VideoCaptureManager::ReleaseDevices() {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  for (auto& controller : controllers_) {
    if (controller->stream_type() != MEDIA_DEVICE_VIDEO_CAPTURE)
      continue;

    DoStopDevice(controller.get());
  }
}
