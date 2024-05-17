void MediaStreamManager::FinalizeMediaAccessRequest(
    const std::string& label,
    DeviceRequest* request,
    const MediaStreamDevices& devices) {
  DCHECK(request->media_access_request_cb);
  std::move(request->media_access_request_cb)
      .Run(devices, std::move(request->ui_proxy));

  DeleteRequest(label);
}
