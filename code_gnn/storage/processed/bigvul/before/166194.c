void MediaStreamManager::ChangeMediaStreamSourceFromBrowser(
    const std::string& label) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  DeviceRequest* request = FindRequest(label);
  if (!request)
    return;

  SetUpDesktopCaptureChangeSourceRequest(request, label);
  IncrementDesktopCaptureCounter(DESKTOP_CAPTURE_NOTIFICATION_CHANGE_SOURCE);
}
