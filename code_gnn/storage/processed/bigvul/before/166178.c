  MediaStreamDispatcherHostTest()
      : thread_bundle_(TestBrowserThreadBundle::IO_MAINLOOP),
        origin_(url::Origin::Create(GURL("https://test.com"))) {
    audio_manager_ = std::make_unique<media::MockAudioManager>(
        std::make_unique<media::TestAudioThread>());
    audio_system_ =
        std::make_unique<media::AudioSystemImpl>(audio_manager_.get());
    browser_context_ = std::make_unique<TestBrowserContext>();
    base::CommandLine::ForCurrentProcess()->AppendSwitch(
        switches::kUseFakeDeviceForMediaStream);
    auto mock_video_capture_provider =
        std::make_unique<MockVideoCaptureProvider>();
    mock_video_capture_provider_ = mock_video_capture_provider.get();
    media_stream_manager_ = std::make_unique<MediaStreamManager>(
        audio_system_.get(), audio_manager_->GetTaskRunner(),
        std::move(mock_video_capture_provider));

    host_ = std::make_unique<MockMediaStreamDispatcherHost>(
        kProcessId, kRenderId, media_stream_manager_.get());
    host_->set_salt_and_origin_callback_for_testing(
        base::BindRepeating(&MediaStreamDispatcherHostTest::GetSaltAndOrigin,
                            base::Unretained(this)));
    host_->SetMediaStreamDeviceObserverForTesting(
        host_->CreateInterfacePtrAndBind());

#if defined(OS_CHROMEOS)
    chromeos::CrasAudioHandler::InitializeForTesting();
#endif
  }
