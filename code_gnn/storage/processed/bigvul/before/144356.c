void ExtensionInstallPrompt::ConfirmReEnable(Delegate* delegate,
                                             const Extension* extension) {
  DCHECK(ui_loop_ == base::MessageLoop::current());
  extension_ = extension;
  delegate_ = delegate;
  bool is_remote_install =
      profile_ &&
      extensions::ExtensionPrefs::Get(profile_)->HasDisableReason(
          extension->id(), extensions::Extension::DISABLE_REMOTE_INSTALL);
  PromptType type = UNSET_PROMPT_TYPE;
  if (is_remote_install)
    type = REMOTE_INSTALL_PROMPT;
  else
    type = RE_ENABLE_PROMPT;
  prompt_ = new Prompt(type);

  LoadImageIfNeeded();
}
