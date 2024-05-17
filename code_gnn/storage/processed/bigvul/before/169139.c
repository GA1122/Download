  content::WebContents* OpenPopupNoOpener(content::RenderFrameHost* opener,
                                          const GURL& url) {
    content::WebContentsAddedObserver popup_observer;
    EXPECT_TRUE(ExecuteScript(
        opener, "window.open('" + url.spec() + "', '', 'noopener')"));
    content::WebContents* popup = popup_observer.GetWebContents();
    WaitForLoadStop(popup);
    return popup;
  }
