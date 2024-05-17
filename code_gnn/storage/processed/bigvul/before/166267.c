void VideoCaptureManager::GetPhotoState(
    int session_id,
    media::VideoCaptureDevice::GetPhotoStateCallback callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  const VideoCaptureController* controller =
      LookupControllerBySessionId(session_id);
  if (!controller)
    return;
  if (controller->IsDeviceAlive()) {
    controller->GetPhotoState(std::move(callback));
    return;
  }
  photo_request_queue_.emplace_back(
      session_id,
      base::Bind(&VideoCaptureController::GetPhotoState,
                 base::Unretained(controller), base::Passed(&callback)));
}
