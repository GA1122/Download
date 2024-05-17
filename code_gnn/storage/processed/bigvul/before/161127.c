void MediaStreamManager::GenerateStream(
    int render_process_id,
    int render_frame_id,
    const std::string& salt,
    int page_request_id,
    const StreamControls& controls,
    const url::Origin& security_origin,
    bool user_gesture,
    GenerateStreamCallback generate_stream_cb,
    DeviceStoppedCallback device_stopped_cb) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  DVLOG(1) << "GenerateStream()";

  DeviceRequest* request =
      new DeviceRequest(render_process_id, render_frame_id, page_request_id,
                        security_origin, user_gesture, MEDIA_GENERATE_STREAM,
                        controls, salt, std::move(device_stopped_cb));

  const std::string& label = AddRequest(request);

  request->generate_stream_cb = std::move(generate_stream_cb);

  if (generate_stream_test_callback_) {
    if (generate_stream_test_callback_.Run(controls)) {
      FinalizeGenerateStream(label, request);
    } else {
      FinalizeRequestFailed(label, request, MEDIA_DEVICE_INVALID_STATE);
    }
    return;
  }

  BrowserThread::PostTask(BrowserThread::IO, FROM_HERE,
                          base::BindOnce(&MediaStreamManager::SetupRequest,
                                         base::Unretained(this), label));
}
