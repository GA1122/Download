bool MediaStreamManager::FindExistingRequestedDevice(
    const DeviceRequest& new_request,
    const MediaStreamDevice& new_device,
    MediaStreamDevice* existing_device,
    MediaRequestState* existing_request_state) const {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  DCHECK(existing_device);
  DCHECK(existing_request_state);

  std::string source_id = GetHMACForMediaDeviceID(
      new_request.salt_and_origin.device_id_salt,
      new_request.salt_and_origin.origin, new_device.id);

  for (const LabeledDeviceRequest& labeled_request : requests_) {
    const DeviceRequest* request = labeled_request.second;
    if (request->requesting_process_id == new_request.requesting_process_id &&
        request->requesting_frame_id == new_request.requesting_frame_id &&
        request->request_type() == new_request.request_type()) {
      for (const MediaStreamDevice& device : request->devices) {
        if (device.id == source_id && device.type == new_device.type) {
          *existing_device = device;
          int effects = existing_device->input.effects();
          FilterAudioEffects(request->controls, &effects);
          EnableHotwordEffect(request->controls, &effects);
          existing_device->input.set_effects(effects);
          *existing_request_state = request->state(device.type);
          return true;
        }
      }
    }
  }
  return false;
}
