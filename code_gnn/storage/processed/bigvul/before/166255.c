void VideoCaptureManager::DisconnectClient(
    VideoCaptureController* controller,
    VideoCaptureControllerID client_id,
    VideoCaptureControllerEventHandler* client_handler,
    media::VideoCaptureError error) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  DCHECK(controller);
  DCHECK(client_handler);

  if (!IsControllerPointerValid(controller)) {
    NOTREACHED();
    return;
  }
  if (error == media::VideoCaptureError::kNone) {
    if (controller->has_received_frames()) {
      LogVideoCaptureEvent(VIDEO_CAPTURE_STOP_CAPTURE_OK);
    } else if (controller->stream_type() == MEDIA_DEVICE_VIDEO_CAPTURE) {
      LogVideoCaptureEvent(
          VIDEO_CAPTURE_STOP_CAPTURE_OK_NO_FRAMES_PRODUCED_BY_DEVICE);
    } else {
      LogVideoCaptureEvent(
          VIDEO_CAPTURE_STOP_CAPTURE_OK_NO_FRAMES_PRODUCED_BY_DESKTOP_OR_TAB);
    }
  } else {
    LogVideoCaptureEvent(VIDEO_CAPTURE_STOP_CAPTURE_DUE_TO_ERROR);
    LogVideoCaptureError(error);
    std::ostringstream string_stream;
    string_stream << "Video capture session stopped with error code "
                  << static_cast<int>(error);
    EmitLogMessage(string_stream.str(), 1);
    for (auto it : sessions_) {
      if (it.second.type == controller->stream_type() &&
          it.second.id == controller->device_id()) {
        for (auto& listener : listeners_)
          listener.Aborted(it.second.type, it.first);
        if (!IsControllerPointerValid(controller))
          return;
        break;
      }
    }
  }

  const media::VideoCaptureSessionId session_id =
      controller->RemoveClient(client_id, client_handler);
  std::ostringstream string_stream;
  string_stream << "DisconnectClient: session_id = " << session_id;
  EmitLogMessage(string_stream.str(), 1);

  DestroyControllerIfNoClients(controller);
}
