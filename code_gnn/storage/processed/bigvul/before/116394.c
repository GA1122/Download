void ChromeContentRendererClient::GetNavigationErrorStrings(
    const WebKit::WebURLRequest& failed_request,
    const WebKit::WebURLError& error,
    std::string* error_html,
    string16* error_description) {
  const GURL failed_url = error.unreachableURL;
  const Extension* extension = NULL;
  const bool is_repost =
      error.reason == net::ERR_CACHE_MISS &&
      error.domain == WebString::fromUTF8(net::kErrorDomain) &&
      EqualsASCII(failed_request.httpMethod(), "POST");

  if (failed_url.is_valid() && !failed_url.SchemeIs(chrome::kExtensionScheme)) {
    extension = extension_dispatcher_->extensions()->GetExtensionOrAppByURL(
        ExtensionURLInfo(failed_url));
  }

  if (error_html) {
    int resource_id;
    DictionaryValue error_strings;
    if (extension && !extension->from_bookmark()) {
      LocalizedError::GetAppErrorStrings(error, failed_url, extension,
                                         &error_strings);

      resource_id = IDR_ERROR_APP_HTML;
    } else {
      if (is_repost) {
        LocalizedError::GetFormRepostStrings(failed_url, &error_strings);
      } else {
        LocalizedError::GetStrings(error, &error_strings);
      }
      resource_id = IDR_NET_ERROR_HTML;
    }

    const base::StringPiece template_html(
        ResourceBundle::GetSharedInstance().GetRawDataResource(resource_id));
    if (template_html.empty()) {
      NOTREACHED() << "unable to load template. ID: " << resource_id;
    } else {
      *error_html = jstemplate_builder::GetTemplatesHtml(
          template_html, &error_strings, "t");
    }
  }

  if (error_description) {
    if (!extension && !is_repost)
      *error_description = LocalizedError::GetErrorDetails(error);
  }
}
