  static void SetUpTestCase() {
    scoped_task_environment = std::make_unique<TestBrowserThreadBundle>(
        TestBrowserThreadBundle::REAL_IO_THREAD);

    io_runner =
        base::CreateSingleThreadTaskRunnerWithTraits({BrowserThread::IO});

    base::Thread::Options options(base::MessageLoop::TYPE_IO, 0);
    background_thread.reset(new base::Thread("AppCacheTest::BackgroundThread"));
    ASSERT_TRUE(background_thread->StartWithOptions(options));
  }
