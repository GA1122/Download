void ShowPageInfoDialogImpl(Browser* browser,
                            content::WebContents* web_contents,
                            const GURL& virtual_url,
                            const security_state::SecurityInfo& security_info) {
  views::BubbleDialogDelegateView* bubble =
      PageInfoBubbleView::CreatePageInfoBubble(browser, web_contents,
                                               virtual_url, security_info);
  BrowserView* browser_view = BrowserView::GetBrowserViewForBrowser(browser);
  bubble->GetWidget()->AddObserver(
      browser_view->GetLocationBarView()->location_icon_view());
  bubble->GetWidget()->Show();
}
