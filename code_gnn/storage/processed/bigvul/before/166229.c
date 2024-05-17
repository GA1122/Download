void MediaStreamManager::SetUpDesktopCaptureChangeSourceRequest(
    DeviceRequest* request,
    const std::string& label) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  DCHECK(IsDesktopCaptureMediaType(request->video_type()));
  DCHECK(request->request_type() == MEDIA_GENERATE_STREAM ||
         request->request_type() == MEDIA_DEVICE_UPDATE);

  request->set_request_type(MEDIA_DEVICE_UPDATE);

  request->CreateUIRequest(std::string()  ,
                           std::string()  );

  ReadOutputParamsAndPostRequestToUI(label, request, MediaDeviceEnumeration());
}
