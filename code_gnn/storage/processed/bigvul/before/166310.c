  VideoCaptureTest()
      : thread_bundle_(content::TestBrowserThreadBundle::IO_MAINLOOP),
        audio_manager_(std::make_unique<media::MockAudioManager>(
            std::make_unique<media::TestAudioThread>())),
        audio_system_(
            std::make_unique<media::AudioSystemImpl>(audio_manager_.get())),
        task_runner_(base::ThreadTaskRunnerHandle::Get()),
        opened_session_id_(kInvalidMediaCaptureSessionId),
        observer_binding_(this) {}
