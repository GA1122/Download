bool RenderFrameImpl::IsPluginHandledExternally(
    const blink::WebElement& plugin_element,
    const blink::WebURL& url,
    const blink::WebString& suggested_mime_type) {
#if BUILDFLAG(ENABLE_PLUGINS)
  if (!BrowserPluginManager::Get()) {
    return false;
  }
  int32_t tentative_element_instance_id =
      BrowserPluginManager::Get()->GetNextInstanceID();
  return GetContentClient()->renderer()->MaybeCreateMimeHandlerView(
      this, plugin_element, GURL(url), suggested_mime_type.Utf8(),
      tentative_element_instance_id);
#else
  return false;
#endif
}
