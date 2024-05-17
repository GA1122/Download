bool NeedsExtensionWebUI(content::WebUI* web_ui,
                         Profile* profile,
                         const GURL& url) {
  ExtensionService* service = profile ? profile->GetExtensionService() : NULL;
  return service && service->ExtensionBindingsAllowed(url) &&
      (!web_ui ||
        TabContentsWrapper::GetCurrentWrapperForContents(
            web_ui->GetWebContents()));
}
