void InProcessBrowserTest::RunTestOnMainThreadLoop() {
#if defined(OS_POSIX)
  signal(SIGTERM, DumpStackTraceSignalHandler);
#endif   

#if defined(OS_MACOSX)
  base::mac::ScopedNSAutoreleasePool pool;
#endif

  ui_test_utils::RunAllPendingInMessageLoop();

#if defined(OS_MACOSX)
  pool.Recycle();
#endif

  browser_ = CreateBrowser(ProfileManager::GetDefaultProfile());
#if defined(OS_MACOSX)
  pool.Recycle();
#endif

  ui_test_utils::RunAllPendingInMessageLoop();

  SetUpOnMainThread();
#if defined(OS_MACOSX)
  pool.Recycle();
#endif

  if (!HasFatalFailure())
    RunTestOnMainThread();
#if defined(OS_MACOSX)
  pool.Recycle();
#endif

  CleanUpOnMainThread();
#if defined(OS_MACOSX)
  pool.Recycle();
#endif

  QuitBrowsers();
#if defined(OS_MACOSX)
  pool.Recycle();
#endif
}
