MediaStreamDevices MediaStreamManager::GetDevicesOpenedByRequest(
    const std::string& label) const {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  DeviceRequest* request = FindRequest(label);
  if (!request)
    return MediaStreamDevices();
  return request->devices;
}
