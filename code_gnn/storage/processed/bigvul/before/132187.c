blink::WebPlugin* RenderFrameImpl::CreatePlugin(
    blink::WebFrame* frame,
    const WebPluginInfo& info,
    const blink::WebPluginParams& params,
    scoped_ptr<content::PluginInstanceThrottler> throttler) {
  DCHECK_EQ(frame_, frame);
#if defined(ENABLE_PLUGINS)
  if (info.type == WebPluginInfo::PLUGIN_TYPE_BROWSER_PLUGIN) {
    return BrowserPluginManager::Get()->CreateBrowserPlugin(
        this, GetContentClient()
                  ->renderer()
                  ->CreateBrowserPluginDelegate(this, params.mimeType.utf8(),
                                                GURL(params.url))
                  ->GetWeakPtr());
  }

  bool pepper_plugin_was_registered = false;
  scoped_refptr<PluginModule> pepper_module(PluginModule::Create(
      this, info, &pepper_plugin_was_registered));
  if (pepper_plugin_was_registered) {
    if (pepper_module.get()) {
      return new PepperWebPluginImpl(
          pepper_module.get(), params, this,
          make_scoped_ptr(
              static_cast<PluginInstanceThrottlerImpl*>(throttler.release())));
    }
  }
#if defined(OS_CHROMEOS)
  LOG(WARNING) << "Pepper module/plugin creation failed.";
#else
  if (info.type == WebPluginInfo::PLUGIN_TYPE_NPAPI) {
    return new WebPluginImpl(frame, params, info.path, render_view_, this);
  }
#endif
#endif
  return NULL;
}
