v8::Local<v8::Object> RenderFrameImpl::GetScriptableObject(
    const blink::WebElement& plugin_element,
    v8::Isolate* isolate) {
#if BUILDFLAG(ENABLE_PLUGINS)
  if (!content::MimeHandlerViewMode::UsesCrossProcessFrame())
    return v8::Local<v8::Object>();

  return GetContentClient()->renderer()->GetScriptableObject(plugin_element,
                                                             isolate);
#else
  return v8::Local<v8::Object>();
#endif
}
