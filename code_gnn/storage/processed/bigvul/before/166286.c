void VideoCaptureManager::QueueStartDevice(
    media::VideoCaptureSessionId session_id,
    VideoCaptureController* controller,
    const media::VideoCaptureParams& params) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  device_start_request_queue_.push_back(
      CaptureDeviceStartRequest(controller, session_id, params));
  if (device_start_request_queue_.size() == 1)
    ProcessDeviceStartRequestQueue();
}
