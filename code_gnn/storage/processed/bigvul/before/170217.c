  void FinishVideoTest() {
    content::MediaStreamRequest request(CreateRequest(
        content::MEDIA_NO_SERVICE, content::MEDIA_DEVICE_VIDEO_CAPTURE));
    MediaStreamDevicesController::RequestPermissions(
        request, base::Bind(&MediaStreamDevicesControllerBrowserTest::Accept,
                            base::Unretained(this)));

    base::RunLoop::QuitCurrentWhenIdleDeprecated();
  }
