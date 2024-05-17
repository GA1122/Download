void RenderFrameImpl::RenderWidgetSetFocus(bool enable) {
#if BUILDFLAG(ENABLE_PLUGINS)
  for (auto* plugin : active_pepper_instances_)
    plugin->SetContentAreaFocus(enable);
#endif
}
