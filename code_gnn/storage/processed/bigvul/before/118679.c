void InProcessBrowserTest::RunTestOnMainThreadLoop() {
  content::RunAllPendingInMessageLoop();

  chrome::HostDesktopType active_desktop = chrome::GetActiveDesktop();
  scoped_ptr<SingleDesktopTestObserver> single_desktop_test_observer;
  if (!multi_desktop_test_) {
    single_desktop_test_observer.reset(
        new SingleDesktopTestObserver(active_desktop));
  }

  const BrowserList* active_browser_list =
      BrowserList::GetInstance(active_desktop);
  if (!active_browser_list->empty()) {
    browser_ = active_browser_list->get(0);
#if defined(USE_ASH)
    if (browser_->window()->IsMaximized())
      browser_->window()->Restore();
#endif
    content::WaitForLoadStop(
        browser_->tab_strip_model()->GetActiveWebContents());
  }

#if !defined(OS_ANDROID) && !defined(OS_IOS)
  ASSERT_TRUE(storage_monitor::TestStorageMonitor::CreateForBrowserTests());
#endif

#if defined(OS_MACOSX)
  autorelease_pool_ = new base::mac::ScopedNSAutoreleasePool;
#endif

  content::RunAllPendingInMessageLoop();

  SetUpOnMainThread();
#if defined(OS_MACOSX)
  autorelease_pool_->Recycle();
#endif

  if (!HasFatalFailure())
    RunTestOnMainThread();
#if defined(OS_MACOSX)
  autorelease_pool_->Recycle();
#endif

  CleanUpOnMainThread();
#if defined(OS_MACOSX)
  autorelease_pool_->Recycle();
#endif

  content::RunAllPendingInMessageLoop();

  QuitBrowsers();
  for (chrome::HostDesktopType t = chrome::HOST_DESKTOP_TYPE_FIRST;
       t < chrome::HOST_DESKTOP_TYPE_COUNT;
       t = static_cast<chrome::HostDesktopType>(t + 1)) {
    CHECK(BrowserList::GetInstance(t)->empty()) << t;
  }
}
