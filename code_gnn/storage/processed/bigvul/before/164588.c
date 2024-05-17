void MediaStreamDispatcherHost::OnDeviceChanged(
    const std::string& label,
    const blink::MediaStreamDevice& old_device,
    const blink::MediaStreamDevice& new_device) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  GetMediaStreamDeviceObserver()->OnDeviceChanged(label, old_device,
                                                  new_device);
}
