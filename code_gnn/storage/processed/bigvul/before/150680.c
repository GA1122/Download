void PageInfo::OpenSiteSettingsView() {
#if defined(OS_ANDROID)
  NOTREACHED();
#else
  chrome::ShowSiteSettings(chrome::FindBrowserWithWebContents(web_contents()),
                           site_url());
  RecordPageInfoAction(PageInfo::PAGE_INFO_SITE_SETTINGS_OPENED);
#endif
}
