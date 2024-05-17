  void GetRawNondefaultIdOnIOThread(std::string* out) {
    DCHECK_CURRENTLY_ON(BrowserThread::IO);
    MediaDevicesManager::BoolDeviceTypes devices_to_enumerate;
    devices_to_enumerate[MEDIA_DEVICE_TYPE_AUDIO_OUTPUT] = true;

    media_stream_manager_->media_devices_manager()->EnumerateDevices(
        devices_to_enumerate,
        base::Bind(
            [](std::string* out, const MediaDeviceEnumeration& result) {
              CHECK(result[MediaDeviceType::MEDIA_DEVICE_TYPE_AUDIO_OUTPUT]
                        .size() > 1)
                  << "Expected to have a nondefault device.";
              *out = result[MediaDeviceType::MEDIA_DEVICE_TYPE_AUDIO_OUTPUT][1]
                         .device_id;
            },
            base::Unretained(out)));
  }
