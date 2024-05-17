  AudioOutputDeviceTest()
      : default_audio_parameters_(AudioParameters::AUDIO_PCM_LINEAR,
                                  CHANNEL_LAYOUT_STEREO,
                                  48000, 16, 1024),
        audio_device_(new AudioOutputDevice(
            &audio_output_ipc_, io_loop_.message_loop_proxy())) {
  }
