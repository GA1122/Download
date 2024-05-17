void RenderViewTest::TearDown() {
  const int kGCIterations = 5;
  for (int i = 0; i < kGCIterations; i++)
    GetMainFrame()->collectGarbage();

  ProcessPendingMessages();

  for (int i = 0; i < kGCIterations; i++)
    GetMainFrame()->collectGarbage();

  render_thread_->SendCloseMessage();
  view_ = NULL;
  mock_process_.reset();

  base::RunLoop().RunUntilIdle();

#if defined(OS_MACOSX)
  autorelease_pool_.reset(NULL);
#endif

  blink_platform_impl_.Scheduler()->Shutdown();
  blink::shutdown();

  platform_->PlatformUninitialize();
  platform_.reset();
  params_.reset();
  command_line_.reset();
}
