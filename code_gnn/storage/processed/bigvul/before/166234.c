bool MediaStreamManager::SetUpTabCaptureRequest(DeviceRequest* request,
                                                const std::string& label) {
  DCHECK(request->audio_type() == MEDIA_GUM_TAB_AUDIO_CAPTURE ||
         request->video_type() == MEDIA_GUM_TAB_VIDEO_CAPTURE);

  std::string capture_device_id;
  if (!request->controls.audio.device_id.empty()) {
    capture_device_id = request->controls.audio.device_id;
  } else if (!request->controls.video.device_id.empty()) {
    capture_device_id = request->controls.video.device_id;
  } else {
    return false;
  }

  if ((request->audio_type() != MEDIA_GUM_TAB_AUDIO_CAPTURE &&
       request->audio_type() != MEDIA_NO_SERVICE) ||
      (request->video_type() != MEDIA_GUM_TAB_VIDEO_CAPTURE &&
       request->video_type() != MEDIA_NO_SERVICE)) {
    return false;
  }

  base::PostTaskWithTraitsAndReplyWithResult(
      FROM_HERE, {BrowserThread::UI},
      base::BindOnce(&MediaStreamManager::ResolveTabCaptureDeviceIdOnUIThread,
                     base::Unretained(this), capture_device_id,
                     request->requesting_process_id,
                     request->requesting_frame_id,
                     request->salt_and_origin.origin.GetURL()),
      base::BindOnce(
          &MediaStreamManager::FinishTabCaptureRequestSetupWithDeviceId,
          base::Unretained(this), label));
  return true;
}
