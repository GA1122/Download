bool MediaStreamManager::SetupTabCaptureRequest(DeviceRequest* request) {
  DCHECK(request->audio_type() == MEDIA_TAB_AUDIO_CAPTURE ||
         request->video_type() == MEDIA_TAB_VIDEO_CAPTURE);

  std::string capture_device_id;
  if (!request->controls.audio.device_id.empty()) {
    capture_device_id = request->controls.audio.device_id;
  } else if (!request->controls.video.device_id.empty()) {
    capture_device_id = request->controls.video.device_id;
  } else {
    return false;
  }

  WebContentsMediaCaptureId web_id;
  bool has_valid_device_id =
      WebContentsMediaCaptureId::Parse(capture_device_id, &web_id);
  if (!has_valid_device_id ||
      (request->audio_type() != MEDIA_TAB_AUDIO_CAPTURE &&
       request->audio_type() != MEDIA_NO_SERVICE) ||
      (request->video_type() != MEDIA_TAB_VIDEO_CAPTURE &&
       request->video_type() != MEDIA_NO_SERVICE)) {
    return false;
  }
  web_id.disable_local_echo = request->controls.disable_local_echo;

  request->tab_capture_device_id = web_id.ToString();

  request->CreateTabCaptureUIRequest(web_id.render_process_id,
                                     web_id.main_render_frame_id);

  DVLOG(3) << "SetupTabCaptureRequest "
           << ", {capture_device_id = " << capture_device_id << "}"
           << ", {target_render_process_id = " << web_id.render_process_id
           << "}"
           << ", {target_render_frame_id = " << web_id.main_render_frame_id
           << "}";
  return true;
}
