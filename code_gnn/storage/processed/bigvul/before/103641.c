bool ChromeContentRendererClient::CrossesExtensionExtents(WebFrame* frame,
                                                          const GURL& new_url) {
  const ExtensionSet* extensions = extension_dispatcher_->extensions();
  GURL old_url(frame->document().url());
  if (old_url.is_empty() && frame->opener())
    old_url = frame->opener()->document().url();

  if (old_url == new_url) {
    bool is_extension_url = !!extensions->GetByURL(new_url);
    if (is_extension_url != extension_dispatcher_->is_extension_process())
      return true;
  }

  return !extensions->InSameExtent(old_url, new_url);
}
