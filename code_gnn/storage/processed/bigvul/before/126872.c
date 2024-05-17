void BrowserView::ShowWebsiteSettings(Profile* profile,
                                      TabContents* tab_contents,
                                      const GURL& url,
                                      const content::SSLStatus& ssl,
                                      bool show_history) {
  WebsiteSettingsPopupView::ShowPopup(
      GetLocationBarView()->location_icon_view(), profile,
      tab_contents, url, ssl, browser_.get());
}
