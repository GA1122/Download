 void MediaStreamDispatcherHost::OnDeviceStopped(
    const std::string& label,
    const MediaStreamDevice& device) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  GetMediaStreamDeviceObserver()->OnDeviceStopped(label, device);
}
