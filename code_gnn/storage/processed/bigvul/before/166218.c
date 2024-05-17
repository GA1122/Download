void MediaStreamManager::Opened(MediaStreamType stream_type,
                                int capture_session_id) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  DVLOG(1) << "Opened({stream_type = " << stream_type << "} "
           << "{capture_session_id = " << capture_session_id << "})";

  for (const LabeledDeviceRequest& labeled_request : requests_) {
    const std::string& label = labeled_request.first;
    DeviceRequest* request = labeled_request.second;
    for (MediaStreamDevice& device : request->devices) {
      if (device.type == stream_type &&
          device.session_id == capture_session_id) {
        CHECK_EQ(request->state(device.type), MEDIA_REQUEST_STATE_OPENING);
        request->SetState(device.type, MEDIA_REQUEST_STATE_DONE);

        if (IsAudioInputMediaType(device.type)) {
          if (device.type != MEDIA_GUM_TAB_AUDIO_CAPTURE) {
            const MediaStreamDevice* opened_device =
                audio_input_device_manager_->GetOpenedDeviceById(
                    device.session_id);
            device.input = opened_device->input;

            int effects = device.input.effects();
            FilterAudioEffects(request->controls, &effects);
            EnableHotwordEffect(request->controls, &effects);
            device.input.set_effects(effects);
          }
        }
        if (RequestDone(*request))
          HandleRequestDone(label, request);
        break;
      }
    }
  }
}
