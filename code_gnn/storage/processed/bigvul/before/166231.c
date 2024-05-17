bool MediaStreamManager::SetUpDisplayCaptureRequest(DeviceRequest* request) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  DCHECK(request->video_type() == MEDIA_DISPLAY_VIDEO_CAPTURE);

  if (!request->controls.video.requested ||
      !request->controls.video.device_id.empty()) {
    LOG(ERROR) << "Invalid display media request.";
    return false;
  }

  request->CreateUIRequest(std::string()  ,
                           std::string()  );
  DVLOG(3) << "Audio requested " << request->controls.audio.requested
           << " Video requested " << request->controls.video.requested;
  return true;
}
