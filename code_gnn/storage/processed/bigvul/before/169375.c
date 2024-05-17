void WaitForLoadStopWithoutSuccessCheck(WebContents* web_contents) {
  if (web_contents->IsLoading()) {
    WindowedNotificationObserver load_stop_observer(
        NOTIFICATION_LOAD_STOP,
        Source<NavigationController>(&web_contents->GetController()));
    load_stop_observer.Wait();
  }
}
