void WebsiteSettingsPopupView::ShowPopup(views::View* anchor_view,
                                         const gfx::Rect& anchor_rect,
                                         Profile* profile,
                                         content::WebContents* web_contents,
                                         const GURL& url,
                                         const content::SSLStatus& ssl) {
  is_popup_showing = true;
  gfx::NativeView parent_window =
      anchor_view ? nullptr : web_contents->GetNativeView();
  if (InternalChromePage(url)) {
    InternalPageInfoPopupView* popup =
        new InternalPageInfoPopupView(anchor_view, parent_window);
    if (!anchor_view)
      popup->SetAnchorRect(anchor_rect);
    popup->GetWidget()->Show();
  } else {
    WebsiteSettingsPopupView* popup = new WebsiteSettingsPopupView(
        anchor_view, parent_window, profile, web_contents, url, ssl);
    if (!anchor_view)
      popup->SetAnchorRect(anchor_rect);
    popup->GetWidget()->Show();
  }
}
