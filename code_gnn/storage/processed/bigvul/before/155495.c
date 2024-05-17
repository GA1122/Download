void AddPreviewNavigationToBlackListCallback(
    content::BrowserContext* browser_context,
    const GURL& url,
    previews::PreviewsType type,
    uint64_t page_id,
    bool opt_out) {
  PreviewsService* previews_service = PreviewsServiceFactory::GetForProfile(
      Profile::FromBrowserContext(browser_context));
  if (previews_service && previews_service->previews_ui_service()) {
    previews_service->previews_ui_service()->AddPreviewNavigation(
        url, type, opt_out, page_id);
  }
}
