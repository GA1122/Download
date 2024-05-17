void CaptureVisibleTabFunction::Observe(NotificationType type,
                                        const NotificationSource& source,
                                        const NotificationDetails& details) {
  DCHECK(type == NotificationType::TAB_SNAPSHOT_TAKEN);

  const SkBitmap *screen_capture = Details<const SkBitmap>(details).ptr();
  const bool error = screen_capture->empty();

  if (error) {
    error_ = keys::kInternalVisibleTabCaptureError;
    SendResponse(false);
  } else {
    SendResultFromBitmap(*screen_capture);
  }

  Release();   
}
