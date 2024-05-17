void AudioOutputDeviceTest::StopAudioDevice() {
  EXPECT_CALL(audio_output_ipc_, CloseStream(kStreamId));
  EXPECT_CALL(audio_output_ipc_, RemoveDelegate(kStreamId));

  audio_device_->Stop();
  io_loop_.RunAllPending();
}
