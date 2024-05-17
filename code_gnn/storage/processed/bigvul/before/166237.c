void MediaStreamManager::StopDevice(MediaStreamType type, int session_id) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  DVLOG(1) << "StopDevice"
           << "{type = " << type << "}"
           << "{session_id = " << session_id << "}";
  auto request_it = requests_.begin();
  while (request_it != requests_.end()) {
    DeviceRequest* request = request_it->second;
    MediaStreamDevices* devices = &request->devices;
    if (devices->empty()) {
      ++request_it;
      continue;
    }
    auto device_it = devices->begin();
    while (device_it != devices->end()) {
      if (device_it->type != type || device_it->session_id != session_id) {
        ++device_it;
        continue;
      }

      if (request->state(type) == MEDIA_REQUEST_STATE_DONE)
        CloseDevice(type, session_id);
      device_it = devices->erase(device_it);
    }

    if (devices->empty()) {
      std::string label = request_it->first;
      ++request_it;
      DeleteRequest(label);
    } else {
      ++request_it;
    }
  }
}
