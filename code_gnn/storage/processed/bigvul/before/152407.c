bool RenderFrameImpl::MaybeCreateMimeHandlerView(
    const blink::WebElement& plugin_element,
    const blink::WebURL& url,
    const blink::WebString& suggested_mime_type) {
  DCHECK(content::MimeHandlerViewMode::UsesCrossProcessFrame());
#if BUILDFLAG(ENABLE_PLUGINS)
  return GetContentClient()->renderer()->MaybeCreateMimeHandlerView(
      this, plugin_element, GURL(url), suggested_mime_type.Utf8());
#else
  return false;
#endif
}
