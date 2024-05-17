bool Document::CanExecuteScripts(ReasonForCallingCanExecuteScripts reason) {
  DCHECK(GetFrame())
      << "you are querying canExecuteScripts on a non contextDocument.";

  if (IsSandboxed(kSandboxScripts) &&
      !ContentSecurityPolicy::ShouldBypassMainWorld(this)) {
    if (reason == kAboutToExecuteScript) {
      AddConsoleMessage(ConsoleMessage::Create(
          kSecurityMessageSource, mojom::ConsoleMessageLevel::kError,
          "Blocked script execution in '" + Url().ElidedString() +
              "' because the document's frame is sandboxed and the "
              "'allow-scripts' permission is not set."));
    }
    return false;
  }

  if (!GetFrame()->Client())
    return false;

  WebContentSettingsClient* settings_client =
      GetFrame()->GetContentSettingsClient();

  Settings* settings = GetFrame()->GetSettings();
  bool script_enabled = settings && settings->GetScriptEnabled();
  if (settings_client)
    script_enabled = settings_client->AllowScript(script_enabled);
  if (!script_enabled && reason == kAboutToExecuteScript && settings_client)
    settings_client->DidNotAllowScript();
  return script_enabled;
}
