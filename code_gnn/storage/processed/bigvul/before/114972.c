void TestingAutomationProvider::InstallExtension(
    DictionaryValue* args, IPC::Message* reply_message) {
  FilePath::StringType path_string;
  bool with_ui;
  if (!args->GetString("path", &path_string)) {
    AutomationJSONReply(this, reply_message).SendError(
        "Missing or invalid 'path'");
    return;
  }
  if (!args->GetBoolean("with_ui", &with_ui)) {
    AutomationJSONReply(this, reply_message).SendError(
        "Missing or invalid 'with_ui'");
    return;
  }
  ExtensionService* service = profile()->GetExtensionService();
  ExtensionProcessManager* manager = profile()->GetExtensionProcessManager();
  if (service && manager) {
    new ExtensionReadyNotificationObserver(
        manager,
        service,
        this,
        reply_message);

    FilePath extension_path(path_string);
    if (extension_path.MatchesExtension(FILE_PATH_LITERAL(".crx"))) {
      ExtensionInstallUI* client =
          (with_ui ? new ExtensionInstallUI(profile()) : NULL);
      scoped_refptr<CrxInstaller> installer(
          CrxInstaller::Create(service, client));
      if (!with_ui)
        installer->set_allow_silent_install(true);
      installer->set_install_cause(extension_misc::INSTALL_CAUSE_AUTOMATION);
      installer->InstallCrx(extension_path);
    } else {
      scoped_refptr<extensions::UnpackedInstaller> installer(
          extensions::UnpackedInstaller::Create(service));
      installer->set_prompt_for_plugins(with_ui);
      installer->Load(extension_path);
    }
  } else {
    AutomationJSONReply(this, reply_message).SendError(
        "Extensions service/process manager is not available");
  }
}
