blink::WebPlugin* RenderFrameImpl::CreatePlugin(
    const WebPluginInfo& info,
    const blink::WebPluginParams& params,
    std::unique_ptr<content::PluginInstanceThrottler> throttler) {
#if BUILDFLAG(ENABLE_PLUGINS)
  if (info.type == WebPluginInfo::PLUGIN_TYPE_BROWSER_PLUGIN) {
    BrowserPluginDelegate* delegate =
        GetContentClient()->renderer()->CreateBrowserPluginDelegate(
            this, params.mime_type.Utf8(), GURL(params.url));
    return BrowserPluginManager::Get()->CreateBrowserPlugin(
        this, delegate->GetWeakPtr());
  }

  bool pepper_plugin_was_registered = false;
  scoped_refptr<PluginModule> pepper_module(PluginModule::Create(
      this, info, &pepper_plugin_was_registered));
  if (pepper_plugin_was_registered) {
    if (pepper_module.get()) {
      return new PepperWebPluginImpl(
          pepper_module.get(), params, this,
          base::WrapUnique(
              static_cast<PluginInstanceThrottlerImpl*>(throttler.release())));
    }
  }
#if defined(OS_CHROMEOS)
  LOG(WARNING) << "Pepper module/plugin creation failed.";
#endif
#endif   
  return nullptr;
}
