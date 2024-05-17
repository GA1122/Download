void DevToolsUIBindings::FrontendLoaded() {
  if (frontend_loaded_)
    return;
  frontend_loaded_ = true;

  delegate_->OnLoadCompleted();

  AddDevToolsExtensionsToClient();
}
