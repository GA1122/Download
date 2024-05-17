ExtensionSettingsHandler::~ExtensionSettingsHandler() {
  if (load_extension_dialog_.get())
    load_extension_dialog_->ListenerDestroyed();

  registrar_.RemoveAll();
}
