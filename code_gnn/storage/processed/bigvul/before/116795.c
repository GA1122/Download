void WebRTCAudioDeviceTest::InitializeIOThread(const char* thread_name) {
  initialize_com_.reset(new ScopedCOMInitializer());

  io_thread_.reset(new content::TestBrowserThread(content::BrowserThread::IO,
                                                  MessageLoop::current()));

  audio_manager_.reset(AudioManager::Create());

  test_request_context_ = new TestURLRequestContext();
  resource_context_->set_request_context(test_request_context_.get());
  media_observer_.reset(new MockMediaObserver());
  resource_context_->set_media_observer(media_observer_.get());

  CreateChannel(thread_name);
}
