void VideoInputDevicesEnumerated(base::Closure quit_closure,
                                 const std::string& salt,
                                 const url::Origin& security_origin,
                                 MediaDeviceInfoArray* out,
                                 const MediaDeviceEnumeration& enumeration) {
  for (const auto& info : enumeration[MEDIA_DEVICE_TYPE_VIDEO_INPUT]) {
    std::string device_id = MediaStreamManager::GetHMACForMediaDeviceID(
        salt, security_origin, info.device_id);
    out->push_back(MediaDeviceInfo(device_id, info.label, std::string()));
  }
  std::move(quit_closure).Run();
}
