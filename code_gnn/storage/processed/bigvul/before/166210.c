void MediaStreamManager::HandleAccessRequestResponse(
    const std::string& label,
    const media::AudioParameters& output_parameters,
    const MediaStreamDevices& devices,
    MediaStreamRequestResult result) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  DVLOG(1) << "HandleAccessRequestResponse("
           << ", {label = " << label << "})";

  DeviceRequest* request = FindRequest(label);
  if (!request) {
    return;
  }

  if (request->request_type() == MEDIA_DEVICE_ACCESS) {
    FinalizeMediaAccessRequest(label, request, devices);
    return;
  }

  if (result != MEDIA_DEVICE_OK) {
    FinalizeRequestFailed(label, request, result);
    return;
  }
  DCHECK(!devices.empty());

  if (request->request_type() == MEDIA_DEVICE_UPDATE) {
    HandleChangeSourceRequestResponse(label, request, devices);
    return;
  }

  bool found_audio = false;
  bool found_video = false;
  for (const MediaStreamDevice& media_stream_device : devices) {
    MediaStreamDevice device = media_stream_device;

    if (device.type == MEDIA_GUM_TAB_VIDEO_CAPTURE ||
        device.type == MEDIA_GUM_TAB_AUDIO_CAPTURE) {
      device.id = request->tab_capture_device_id;
    }

    if (device.type == MEDIA_GUM_TAB_AUDIO_CAPTURE ||
        device.type == MEDIA_GUM_DESKTOP_AUDIO_CAPTURE) {
      int sample_rate = output_parameters.sample_rate();
      if (sample_rate <= 0 || sample_rate > 96000)
        sample_rate = 44100;

      media::AudioParameters params(device.input.format(),
                                    media::CHANNEL_LAYOUT_STEREO, sample_rate,
                                    device.input.frames_per_buffer());
      params.set_effects(device.input.effects());
      params.set_mic_positions(device.input.mic_positions());
      DCHECK(params.IsValid());
      device.input = params;
    }

    if (device.type == request->audio_type())
      found_audio = true;
    else if (device.type == request->video_type())
      found_video = true;

    if (request->request_type() == MEDIA_GENERATE_STREAM) {
      MediaRequestState state;
      if (FindExistingRequestedDevice(*request, device, &device, &state)) {
        request->devices.push_back(device);
        request->SetState(device.type, state);
        DVLOG(1) << "HandleAccessRequestResponse - device already opened "
                 << ", {label = " << label << "}"
                 << ", device_id = " << device.id << "}";
        continue;
      }
    }
    device.session_id = GetDeviceManager(device.type)->Open(device);
    TranslateDeviceIdToSourceId(request, &device);
    request->devices.push_back(device);

    request->SetState(device.type, MEDIA_REQUEST_STATE_OPENING);
    DVLOG(1) << "HandleAccessRequestResponse - opening device "
             << ", {label = " << label << "}"
             << ", {device_id = " << device.id << "}"
             << ", {session_id = " << device.session_id << "}";
  }
