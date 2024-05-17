void RenderViewTest::TearDown() {
  GetMainFrame()->collectGarbage();
  GetMainFrame()->collectGarbage();

  ProcessPendingMessages();

  render_thread_->SendCloseMessage();
  view_ = NULL;
  mock_process_.reset();

  msg_loop_.RunAllPending();

  WebKit::shutdown();

  mock_keyboard_.reset();

  platform_->PlatformUninitialize();
  platform_.reset();
  params_.reset();
  command_line_.reset();
}
