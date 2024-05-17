void WebRTCAudioDeviceTest::UninitializeIOThread() {
  resource_context_.reset();

  audio_manager_.reset();
  test_request_context_ = NULL;
  initialize_com_.reset();
}
