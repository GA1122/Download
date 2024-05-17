bool ChromeContentRendererClient::CrossesExtensionExtents(
    WebFrame* frame,
    const GURL& new_url,
    const ExtensionSet& extensions,
    bool is_extension_url,
    bool is_initial_navigation) {
  GURL old_url(frame->top()->document().url());

  if (is_initial_navigation && old_url.is_empty() && frame->opener()) {
    WebDocument opener_document = frame->opener()->document();
    GURL opener_url = opener_document.url();
    WebSecurityOrigin opener_origin = opener_document.securityOrigin();
    bool opener_is_extension_url = !!extensions.GetExtensionOrAppByURL(
        ExtensionURLInfo(opener_origin, opener_url));
    WebSecurityOrigin opener = frame->opener()->document().securityOrigin();
    if (!is_extension_url &&
        !opener_is_extension_url &&
        extension_dispatcher_->is_extension_process() &&
        opener.canRequest(WebURL(new_url)))
      return false;

    old_url = frame->top()->opener()->top()->document().url();
  }

  return extensions::CrossesExtensionProcessBoundary(
      extensions, ExtensionURLInfo(old_url), ExtensionURLInfo(new_url));
}
