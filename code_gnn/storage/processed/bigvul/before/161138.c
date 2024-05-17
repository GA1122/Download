std::string MediaStreamManager::MakeMediaAccessRequest(
    int render_process_id,
    int render_frame_id,
    int page_request_id,
    const StreamControls& controls,
    const url::Origin& security_origin,
    MediaAccessRequestCallback callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  DeviceRequest* request = new DeviceRequest(
      render_process_id, render_frame_id, page_request_id, security_origin,
      false,   
      MEDIA_DEVICE_ACCESS, controls, std::string());

  const std::string& label = AddRequest(request);

  request->media_access_request_cb = std::move(callback);
  BrowserThread::PostTask(BrowserThread::IO, FROM_HERE,
                          base::BindOnce(&MediaStreamManager::SetupRequest,
                                         base::Unretained(this), label));
  return label;
}
