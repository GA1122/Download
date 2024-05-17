void MediaStreamManager::CancelRequest(const std::string& label) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  DVLOG(1) << "CancelRequest({label = " << label <<  "})";
  DeviceRequest* request = FindRequest(label);
  if (!request) {
    LOG(ERROR) << "The request with label = " << label  << " does not exist.";
    return;
  }

  for (const MediaStreamDevice& device : request->devices) {
    const MediaRequestState state = request->state(device.type);
    if (state != MEDIA_REQUEST_STATE_OPENING &&
        state != MEDIA_REQUEST_STATE_DONE) {
      continue;
    }
    CloseDevice(device.type, device.session_id);
  }

  request->SetState(NUM_MEDIA_TYPES, MEDIA_REQUEST_STATE_CLOSING);
  DeleteRequest(label);
}
