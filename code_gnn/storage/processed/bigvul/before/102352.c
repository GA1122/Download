void ExtensionHelper::DidCreateDataSource(WebFrame* frame, WebDataSource* ds) {
  if (!frame->parent()) {
    app_icon_fetchers_.clear();
    app_definition_fetcher_.reset(NULL);
  }

  if (g_schedulers.Get().count(frame))
    return;

  g_schedulers.Get()[frame] = new UserScriptIdleScheduler(
      frame, extension_dispatcher_);
}
