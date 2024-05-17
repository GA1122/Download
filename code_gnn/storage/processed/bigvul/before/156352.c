bool ExtensionDevToolsClientHost::Attach() {
  if (!agent_host_->AttachClient(this))
    return false;

  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          ::switches::kSilentDebuggerExtensionAPI)) {
    return true;
  }

  if (Manifest::IsPolicyLocation(extension_->location()))
    return true;

  infobar_ = ExtensionDevToolsInfoBar::Create(
      extension_id(), extension_->name(), this,
      base::Bind(&ExtensionDevToolsClientHost::InfoBarDismissed,
                 base::Unretained(this)));
  return true;
}
