void InProcessBrowserTest::QuitBrowsers() {
  if (BrowserList::size() == 0)
    return;

  MessageLoopForUI::current()->PostTask(FROM_HERE,
                                        base::Bind(&BrowserList::AttemptExit));
  ui_test_utils::RunMessageLoop();
}
