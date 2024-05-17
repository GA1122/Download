void VideoCaptureManager::TakePhoto(
    int session_id,
    media::VideoCaptureDevice::TakePhotoCallback callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  VideoCaptureController* controller = LookupControllerBySessionId(session_id);
  if (!controller)
    return;
  if (controller->IsDeviceAlive()) {
    controller->TakePhoto(std::move(callback));
    return;
  }
  photo_request_queue_.emplace_back(
      session_id,
      base::Bind(&VideoCaptureController::TakePhoto,
                 base::Unretained(controller), base::Passed(&callback)));
}
