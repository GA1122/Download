void RenderFrameImpl::WasShown() {
  for (auto& observer : observers_)
    observer.WasShown();

#if BUILDFLAG(ENABLE_PLUGINS)
  for (auto* plugin : active_pepper_instances_)
    plugin->PageVisibilityChanged(true);
#endif   
}
