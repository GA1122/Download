void MediaStreamManager::NotifyDevicesChanged(
    MediaDeviceType device_type,
    const MediaDeviceInfoArray& devices) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  MediaObserver* media_observer =
      GetContentClient()->browser()->GetMediaObserver();

  MediaStreamType stream_type = ConvertToMediaStreamType(device_type);
  MediaStreamDevices new_devices =
      ConvertToMediaStreamDevices(stream_type, devices);

  if (IsAudioInputMediaType(stream_type)) {
    MediaCaptureDevicesImpl::GetInstance()->OnAudioCaptureDevicesChanged(
        new_devices);
    if (media_observer)
      media_observer->OnAudioCaptureDevicesChanged();
  } else if (IsVideoMediaType(stream_type)) {
    MediaCaptureDevicesImpl::GetInstance()->OnVideoCaptureDevicesChanged(
        new_devices);
    if (media_observer)
      media_observer->OnVideoCaptureDevicesChanged();
  } else {
    NOTREACHED();
  }
}
