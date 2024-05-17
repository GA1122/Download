MediaStreamDevice MediaStreamDeviceFromFakeDeviceConfig() {
  DesktopMediaID media_id(DesktopMediaID::TYPE_SCREEN, DesktopMediaID::kNullId);

  MediaStreamDevice device(MEDIA_DISPLAY_VIDEO_CAPTURE, media_id.ToString(),
                           media_id.ToString());
  media::mojom::DisplayCaptureSurfaceType display_surface =
      media::mojom::DisplayCaptureSurfaceType::MONITOR;
  device.display_media_info = media::mojom::DisplayMediaInformation::New(
      display_surface, true, media::mojom::CursorCaptureType::NEVER);

  const base::CommandLine* command_line =
      base::CommandLine::ForCurrentProcess();
  if (command_line &&
      command_line->HasSwitch(switches::kUseFakeDeviceForMediaStream)) {
    std::vector<media::FakeVideoCaptureDeviceSettings> config;
    media::FakeVideoCaptureDeviceFactory::
        ParseFakeDevicesConfigFromOptionsString(
            command_line->GetSwitchValueASCII(
                switches::kUseFakeDeviceForMediaStream),
            &config);
    if (config.empty())
      return device;

    DesktopMediaID::Type desktop_media_type = DesktopMediaID::TYPE_NONE;
    switch (config[0].display_media_type) {
      case media::FakeVideoCaptureDevice::DisplayMediaType::ANY:
      case media::FakeVideoCaptureDevice::DisplayMediaType::MONITOR:
        desktop_media_type = DesktopMediaID::TYPE_SCREEN;
        display_surface = media::mojom::DisplayCaptureSurfaceType::MONITOR;
        break;
      case media::FakeVideoCaptureDevice::DisplayMediaType::WINDOW:
        desktop_media_type = DesktopMediaID::TYPE_WINDOW;
        display_surface = media::mojom::DisplayCaptureSurfaceType::WINDOW;
        break;
      case media::FakeVideoCaptureDevice::DisplayMediaType::BROWSER:
        desktop_media_type = DesktopMediaID::TYPE_WEB_CONTENTS;
        display_surface = media::mojom::DisplayCaptureSurfaceType::BROWSER;
        break;
    }
    media_id = DesktopMediaID(desktop_media_type, DesktopMediaID::kFakeId);
  }
  device = MediaStreamDevice(MEDIA_DISPLAY_VIDEO_CAPTURE, media_id.ToString(),
                             media_id.ToString());
  device.display_media_info = media::mojom::DisplayMediaInformation::New(
      display_surface, true, media::mojom::CursorCaptureType::NEVER);
  return device;
}
