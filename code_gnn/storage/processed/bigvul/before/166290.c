void VideoCaptureManager::RequestRefreshFrameForClient(
    VideoCaptureController* controller) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  if (IsControllerPointerValid(controller)) {
    if (!controller->IsDeviceAlive())
      return;
    controller->RequestRefreshFrame();
  }
}
