void MediaStreamManager::OnMediaStreamUIWindowId(
    MediaStreamType video_type,
    const MediaStreamDevices& devices,
    gfx::NativeViewId window_id) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  if (!window_id)
    return;

  if (video_type != MEDIA_GUM_DESKTOP_VIDEO_CAPTURE)
    return;

  for (const MediaStreamDevice& device : devices) {
    if (device.type != MEDIA_GUM_DESKTOP_VIDEO_CAPTURE)
      continue;

    DesktopMediaID media_id = DesktopMediaID::Parse(device.id);
    if (media_id.type == DesktopMediaID::TYPE_WEB_CONTENTS)
      continue;
#if defined(USE_AURA)
    if (media_id.aura_id > DesktopMediaID::kNullId)
      continue;
#endif
    video_capture_manager_->SetDesktopCaptureWindowId(device.session_id,
                                                      window_id);
    break;
  }
}
