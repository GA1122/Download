void ChromeWebUIControllerFactory::GetFaviconForURL(
    Profile* profile,
    FaviconService::GetFaviconRequest* request,
    const GURL& page_url) const {
  if (page_url.SchemeIs(chrome::kExtensionScheme) &&
      page_url.host() != extension_misc::kBookmarkManagerId) {
    ExtensionWebUI::GetFaviconForURL(profile, request, page_url);
  } else {
    history::FaviconData favicon;
    favicon.image_data = scoped_refptr<RefCountedMemory>(
        GetFaviconResourceBytes(page_url));
    favicon.known_icon = favicon.image_data.get() != NULL &&
                             favicon.image_data->size() > 0;
    favicon.icon_type = history::FAVICON;
    request->ForwardResultAsync(request->handle(), favicon);
  }
}
