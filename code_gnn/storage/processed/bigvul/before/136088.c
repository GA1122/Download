void WebsiteSettingsPopupView::HandleLinkClickedAsync(views::Link* source) {
  if (source == cookie_dialog_link_) {
    presenter_->RecordWebsiteSettingsAction(
        WebsiteSettings::WEBSITE_SETTINGS_COOKIES_DIALOG_OPENED);

    if (web_contents_ != NULL)
      new CollectedCookiesViews(web_contents_);
  } else if (source == certificate_dialog_link_) {
    gfx::NativeWindow parent = GetAnchorView() ?
        GetAnchorView()->GetWidget()->GetNativeWindow() : nullptr;
    presenter_->RecordWebsiteSettingsAction(
        WebsiteSettings::WEBSITE_SETTINGS_CERTIFICATE_DIALOG_OPENED);
    ShowCertificateViewerByID(web_contents_, parent, cert_id_);
  } else if (source == help_center_link_) {
    web_contents_->OpenURL(content::OpenURLParams(
        GURL(chrome::kPageInfoHelpCenterURL), content::Referrer(),
        NEW_FOREGROUND_TAB, ui::PAGE_TRANSITION_LINK, false));
    presenter_->RecordWebsiteSettingsAction(
        WebsiteSettings::WEBSITE_SETTINGS_CONNECTION_HELP_OPENED);
  } else if (source == site_settings_link_) {
    web_contents_->OpenURL(content::OpenURLParams(
        GURL(chrome::kChromeUIContentSettingsURL), content::Referrer(),
        NEW_FOREGROUND_TAB, ui::PAGE_TRANSITION_LINK, false));
    presenter_->RecordWebsiteSettingsAction(
        WebsiteSettings::WEBSITE_SETTINGS_SITE_SETTINGS_OPENED);
  } else {
    NOTREACHED();
  }
}
