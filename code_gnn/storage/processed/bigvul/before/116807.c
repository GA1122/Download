void WebRTCAudioDeviceTest::WaitForAudioManagerCompletion() {
  if (audio_manager_.get())
    WaitForMessageLoopCompletion(audio_manager_->GetMessageLoop());
}
