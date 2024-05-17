InterstitialPage* InterstitialPage::Create(WebContents* web_contents,
                                           bool new_navigation,
                                           const GURL& url,
                                           InterstitialPageDelegate* delegate) {
  return new InterstitialPageImpl(
      web_contents,
      static_cast<RenderWidgetHostDelegate*>(
          static_cast<WebContentsImpl*>(web_contents)),
      new_navigation, url, delegate);
}
