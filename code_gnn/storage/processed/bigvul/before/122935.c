void RenderWidgetHostImpl::ActivateDeferredPluginHandles() {
#if !defined(USE_AURA)
  if (view_ == NULL)
    return;

  for (int i = 0; i < static_cast<int>(deferred_plugin_handles_.size()); i++) {
#if defined(TOOLKIT_GTK)
    view_->CreatePluginContainer(deferred_plugin_handles_[i]);
#endif
  }

  deferred_plugin_handles_.clear();
#endif
}
