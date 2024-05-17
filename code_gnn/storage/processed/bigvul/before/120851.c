void RedirectToNtpOrAppsPageIfNecessary(content::WebContents* contents,
                                        SyncPromoUI::Source source) {
  if (source != SyncPromoUI::SOURCE_SETTINGS &&
      source != SyncPromoUI::SOURCE_WEBSTORE_INSTALL) {
    RedirectToNtpOrAppsPage(contents, source);
  }
}
