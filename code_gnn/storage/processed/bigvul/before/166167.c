void MediaStreamDispatcherHost::OnDeviceChanged(
    const std::string& label,
    const MediaStreamDevice& old_device,
    const MediaStreamDevice& new_device) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  GetMediaStreamDeviceObserver()->OnDeviceChanged(label, old_device,
                                                  new_device);
}
