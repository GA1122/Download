void InProcessBrowserTest::QuitBrowsers() {
  if (chrome::GetTotalBrowserCount() == 0) {
    chrome::NotifyAppTerminating();
    return;
  }

  base::MessageLoopForUI::current()->PostTask(FROM_HERE,
                                              base::Bind(&chrome::AttemptExit));
  content::RunMessageLoop();

#if defined(OS_MACOSX)
  content::RunAllPendingInMessageLoop();
  delete autorelease_pool_;
  autorelease_pool_ = NULL;
#endif
}
