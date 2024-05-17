void RenderView::OnWasRestored(bool needs_repainting) {
  RenderWidget::OnWasRestored(needs_repainting);

  if (webview()) {
    webview()->settings()->setMinimumTimerInterval(
        webkit_glue::kForegroundTabTimerInterval);
    webview()->setVisibilityState(visibilityState(), false);
  }

#if defined(OS_MACOSX)
  std::set<WebPluginDelegateProxy*>::iterator plugin_it;
  for (plugin_it = plugin_delegates_.begin();
       plugin_it != plugin_delegates_.end(); ++plugin_it) {
    (*plugin_it)->SetContainerVisibility(true);
  }
#endif   
}
