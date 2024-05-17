void PageInfoBubbleView::HandleLinkClickedAsync(views::Link* source) {
  if (web_contents() == nullptr || web_contents()->IsBeingDestroyed()) {
    return;
  }
  switch (source->id()) {
    case PageInfoBubbleView::VIEW_ID_PAGE_INFO_LINK_SITE_SETTINGS:
      presenter_->OpenSiteSettingsView();
      break;
    case PageInfoBubbleView::VIEW_ID_PAGE_INFO_LINK_COOKIE_DIALOG:
      presenter_->RecordPageInfoAction(
          PageInfo::PAGE_INFO_COOKIES_DIALOG_OPENED);
      new CollectedCookiesViews(web_contents());
      break;
    case PageInfoBubbleView::VIEW_ID_PAGE_INFO_LINK_CERTIFICATE_VIEWER: {
      gfx::NativeWindow top_window = web_contents()->GetTopLevelNativeWindow();
      if (certificate_ && top_window) {
        presenter_->RecordPageInfoAction(
            PageInfo::PAGE_INFO_CERTIFICATE_DIALOG_OPENED);
        ShowCertificateViewer(web_contents(), top_window, certificate_.get());
      }
      break;
    }
    default:
      NOTREACHED();
  }
}
