void VideoCaptureManager::SetPhotoOptions(
    int session_id,
    media::mojom::PhotoSettingsPtr settings,
    media::VideoCaptureDevice::SetPhotoOptionsCallback callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  VideoCaptureController* controller = LookupControllerBySessionId(session_id);
  if (!controller)
    return;
  if (controller->IsDeviceAlive()) {
    controller->SetPhotoOptions(std::move(settings), std::move(callback));
    return;
  }
  photo_request_queue_.emplace_back(
      session_id, base::Bind(&VideoCaptureController::SetPhotoOptions,
                             base::Unretained(controller),
                             base::Passed(&settings), base::Passed(&callback)));
}
