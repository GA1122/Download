void MediaStreamManager::OpenDevice(int render_process_id,
                                    int render_frame_id,
                                    const std::string& salt,
                                    int page_request_id,
                                    const std::string& device_id,
                                    MediaStreamType type,
                                    const url::Origin& security_origin,
                                    OpenDeviceCallback open_device_cb,
                                    DeviceStoppedCallback device_stopped_cb) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  DCHECK(type == MEDIA_DEVICE_AUDIO_CAPTURE ||
         type == MEDIA_DEVICE_VIDEO_CAPTURE);
  DVLOG(1) << "OpenDevice ({page_request_id = " << page_request_id <<  "})";
  StreamControls controls;
  if (IsAudioInputMediaType(type)) {
    controls.audio.requested = true;
    controls.audio.device_id = device_id;
  } else if (IsVideoMediaType(type)) {
    controls.video.requested = true;
    controls.video.device_id = device_id;
  } else {
    NOTREACHED();
  }
  DeviceRequest* request = new DeviceRequest(
      render_process_id, render_frame_id, page_request_id, security_origin,
      false,   
      MEDIA_OPEN_DEVICE_PEPPER_ONLY, controls, salt,
      std::move(device_stopped_cb));

  const std::string& label = AddRequest(request);

  request->open_device_cb = std::move(open_device_cb);
  BrowserThread::PostTask(BrowserThread::IO, FROM_HERE,
                          base::BindOnce(&MediaStreamManager::SetupRequest,
                                         base::Unretained(this), label));
}
