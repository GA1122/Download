void InProcessBrowserTest::AddBlankTabAndShow(Browser* browser) {
  ui_test_utils::WindowedNotificationObserver observer(
      content::NOTIFICATION_LOAD_STOP,
       content::NotificationService::AllSources());
  browser->AddSelectedTabWithURL(
      GURL(chrome::kAboutBlankURL), content::PAGE_TRANSITION_START_PAGE);
  observer.Wait();

  browser->window()->Show();
}
