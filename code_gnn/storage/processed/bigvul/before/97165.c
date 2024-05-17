PassRefPtr<Widget> WebFrameLoaderClient::createPlugin(
    const IntSize& size,  
    HTMLPlugInElement* element,
    const KURL& url,
    const Vector<String>& param_names,
    const Vector<String>& param_values,
    const String& mime_type,
    bool load_manually) {
#if defined(OS_POSIX)
  if (objectContentType(url, mime_type) != ObjectContentNetscapePlugin)
    return NULL;
#endif

  if (!webframe_->client())
    return NULL;

  WebPluginParams params;
  params.url = webkit_glue::KURLToWebURL(url);
  params.mimeType = webkit_glue::StringToWebString(mime_type);
  CopyStringVector(param_names, &params.attributeNames);
  CopyStringVector(param_values, &params.attributeValues);
  params.loadManually = load_manually;

  WebPlugin* webplugin = webframe_->client()->createPlugin(webframe_, params);
  if (!webplugin)
    return NULL;

  RefPtr<WebPluginContainerImpl> container =
      WebPluginContainerImpl::create(element, webplugin);

  if (!webplugin->initialize(container.get()))
    return NULL;

  if (!element->renderer())
    return NULL;

  return container;
}
