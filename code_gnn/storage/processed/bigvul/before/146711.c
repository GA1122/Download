bool Document::CanExecuteScripts(ReasonForCallingCanExecuteScripts reason) {
  if (IsSandboxed(kSandboxScripts)) {
    if (reason == kAboutToExecuteScript) {
      AddConsoleMessage(ConsoleMessage::Create(
          kSecurityMessageSource, kErrorMessageLevel,
          "Blocked script execution in '" + Url().ElidedString() +
              "' because the document's frame is sandboxed and the "
              "'allow-scripts' permission is not set."));
    }
    return false;
  }

  DCHECK(GetFrame())
      << "you are querying canExecuteScripts on a non contextDocument.";

  ContentSettingsClient* settings_client =
      GetFrame()->GetContentSettingsClient();
  if (!settings_client)
    return false;

  Settings* settings = GetFrame()->GetSettings();
  if (!settings_client->AllowScript(settings && settings->GetScriptEnabled())) {
    if (reason == kAboutToExecuteScript)
      settings_client->DidNotAllowScript();

    return false;
  }

  return true;
}
