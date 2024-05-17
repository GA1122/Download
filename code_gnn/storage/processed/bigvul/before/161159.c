void MediaStreamManager::SetCapturingLinkSecured(int render_process_id,
                                                 int session_id,
                                                 MediaStreamType type,
                                                 bool is_secure) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  for (LabeledDeviceRequest& labeled_request : requests_) {
    DeviceRequest* request = labeled_request.second;
    if (request->requesting_process_id != render_process_id)
      continue;

    for (const MediaStreamDevice& device : request->devices) {
      if (device.session_id == session_id && device.type == type) {
        request->SetCapturingLinkSecured(is_secure);
        return;
      }
    }
  }
}
