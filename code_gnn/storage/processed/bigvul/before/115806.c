  void WaitForInterstitial() {
    TabContents* contents = browser()->GetSelectedTabContents();
    ui_test_utils::WindowedNotificationObserver interstitial_observer(
          content::NOTIFICATION_INTERSTITIAL_ATTACHED,
          content::Source<TabContents>(contents));
    if (!InterstitialPage::GetInterstitialPage(contents))
      interstitial_observer.Wait();
  }
