void VideoCaptureManager::ResumeCaptureForClient(
    media::VideoCaptureSessionId session_id,
    const media::VideoCaptureParams& params,
    VideoCaptureController* controller,
    VideoCaptureControllerID client_id,
    VideoCaptureControllerEventHandler* client_handler) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  DCHECK(controller);
  DCHECK(client_handler);

  if (!IsControllerPointerValid(controller))
    NOTREACHED() << "Got Null controller while resuming capture";

  const bool had_active_client = controller->HasActiveClient();
  controller->ResumeClient(client_id, client_handler);
  if (had_active_client || !controller->HasActiveClient())
    return;
  if (!controller->IsDeviceAlive())
    return;
  controller->Resume();
}
