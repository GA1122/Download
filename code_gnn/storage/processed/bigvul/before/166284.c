void VideoCaptureManager::PauseCaptureForClient(
    VideoCaptureController* controller,
    VideoCaptureControllerID client_id,
    VideoCaptureControllerEventHandler* client_handler) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  DCHECK(controller);
  DCHECK(client_handler);
  if (!IsControllerPointerValid(controller))
    NOTREACHED() << "Got Null controller while pausing capture";

  const bool had_active_client = controller->HasActiveClient();
  controller->PauseClient(client_id, client_handler);
  if (!had_active_client || controller->HasActiveClient())
    return;
  if (!controller->IsDeviceAlive())
    return;
  controller->MaybeSuspend();
}
