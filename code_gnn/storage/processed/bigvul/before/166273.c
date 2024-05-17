void VideoCaptureManager::MaybePostDesktopCaptureWindowId(
    media::VideoCaptureSessionId session_id) {
  auto session_it = sessions_.find(session_id);
  if (session_it == sessions_.end())
    return;

  VideoCaptureController* const existing_device =
      LookupControllerByMediaTypeAndDeviceId(session_it->second.type,
                                             session_it->second.id);
  if (!existing_device) {
    DVLOG(2) << "Failed to find an existing screen capture device.";
    return;
  }

  if (!existing_device->IsDeviceAlive()) {
    DVLOG(2) << "Screen capture device not yet started.";
    return;
  }

  DCHECK_EQ(MEDIA_GUM_DESKTOP_VIDEO_CAPTURE, existing_device->stream_type());
  DesktopMediaID id = DesktopMediaID::Parse(existing_device->device_id());
  if (id.is_null())
    return;

  auto window_id_it = notification_window_ids_.find(session_id);
  if (window_id_it == notification_window_ids_.end()) {
    DVLOG(2) << "Notification window id not set for screen capture.";
    return;
  }

  existing_device->SetDesktopCaptureWindowIdAsync(
      window_id_it->second,
      base::BindOnce([](scoped_refptr<VideoCaptureManager>) {},
                     scoped_refptr<VideoCaptureManager>(this)));
  notification_window_ids_.erase(window_id_it);
}
