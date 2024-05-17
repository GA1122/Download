void RenderFrameImpl::RegisterPeripheralPlugin(
    const GURL& content_origin,
    const base::Closure& unthrottle_callback) {
  return plugin_power_saver_helper_->RegisterPeripheralPlugin(
      content_origin, unthrottle_callback);
}
