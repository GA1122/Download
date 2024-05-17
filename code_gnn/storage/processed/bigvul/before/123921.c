void RenderViewImpl::OnWasHidden() {
  RenderWidget::OnWasHidden();

#if defined(OS_ANDROID)
  media_player_manager_->ReleaseMediaResources();
#endif

  if (webview()) {
    webview()->settings()->setMinimumTimerInterval(
        webkit_glue::kBackgroundTabTimerInterval);
    webview()->setVisibilityState(visibilityState(), false);
  }

  pepper_helper_->PageVisibilityChanged(false);

#if defined(OS_MACOSX)
  std::set<WebPluginDelegateProxy*>::iterator plugin_it;
  for (plugin_it = plugin_delegates_.begin();
       plugin_it != plugin_delegates_.end(); ++plugin_it) {
    (*plugin_it)->SetContainerVisibility(false);
  }
#endif   
}
