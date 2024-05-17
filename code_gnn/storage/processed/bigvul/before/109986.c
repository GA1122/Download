void AudioOutputDeviceTest::StartAudioDevice(bool synchronized_io) {
  const int kInputChannels = synchronized_io ? 2 : 0;

  if (synchronized_io) {
    audio_device_->InitializeIO(default_audio_parameters_,
                                kInputChannels,
                                &callback_);
  } else {
    audio_device_->Initialize(default_audio_parameters_, &callback_);
  }

  audio_device_->Start();

  EXPECT_CALL(audio_output_ipc_, AddDelegate(audio_device_.get()))
      .WillOnce(Return(kStreamId));
  EXPECT_CALL(audio_output_ipc_, CreateStream(kStreamId, _, _));

  io_loop_.RunAllPending();
}
