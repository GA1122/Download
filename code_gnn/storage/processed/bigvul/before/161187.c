  MockAudioManager()
      : AudioManagerPlatform(std::make_unique<media::TestAudioThread>(),
                             &fake_audio_log_factory_),
        num_output_devices_(2),
        num_input_devices_(2) {}
