  void CreateUIRequest(const std::string& requested_audio_device_id,
                       const std::string& requested_video_device_id) {
    DCHECK(!ui_request_);
    target_process_id_ = requesting_process_id;
    target_frame_id_ = requesting_frame_id;
    ui_request_.reset(new MediaStreamRequest(
        requesting_process_id, requesting_frame_id, page_request_id,
        salt_and_origin.origin.GetURL(), user_gesture, request_type_,
        requested_audio_device_id, requested_video_device_id, audio_type_,
        video_type_, controls.disable_local_echo));
  }
