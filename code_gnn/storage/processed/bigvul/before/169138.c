  content::WebContents* OpenPopup(content::RenderFrameHost* opener,
                                  const GURL& url,
                                  bool expect_success = true) {
    content::WindowedNotificationObserver popup_observer(
        chrome::NOTIFICATION_TAB_ADDED,
        content::NotificationService::AllSources());
    EXPECT_TRUE(ExecuteScript(
        opener, "window.popup = window.open('" + url.spec() + "')"));
    popup_observer.Wait();
    content::WebContents* popup =
        browser()->tab_strip_model()->GetActiveWebContents();
    WaitForLoadStop(popup);
    if (expect_success)
      EXPECT_EQ(url, popup->GetMainFrame()->GetLastCommittedURL());
    return popup;
  }
