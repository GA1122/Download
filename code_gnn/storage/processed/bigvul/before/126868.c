void BrowserView::ShowPageInfo(content::WebContents* web_contents,
                               const GURL& url,
                               const SSLStatus& ssl,
                               bool show_history) {
  chrome::ShowPageInfoBubble(GetLocationBarView()->location_icon_view(),
      web_contents, url, ssl, show_history, browser_.get());
}
