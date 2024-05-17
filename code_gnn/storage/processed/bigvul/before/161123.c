void MediaStreamManager::FinalizeOpenDevice(const std::string& label,
                                            DeviceRequest* request) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  DCHECK(request->open_device_cb);

  std::move(request->open_device_cb)
      .Run(true  , label, request->devices.front());
}
