void WebRTCAudioDeviceTest::OnGetHardwareInputChannelCount(uint32* channels) {
  EXPECT_TRUE(audio_util_callback_);
  *channels = audio_util_callback_ ?
      audio_util_callback_->GetAudioInputHardwareChannelCount(
          AudioManagerBase::kDefaultDeviceId) : 0;
}
