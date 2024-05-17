ExtensionInstallDialogViewTestBase::CreatePrompt() {
  std::unique_ptr<ExtensionInstallPrompt::Prompt> prompt(
      new ExtensionInstallPrompt::Prompt(prompt_type_));
  prompt->set_extension(extension_);

  std::unique_ptr<ExtensionIconManager> icon_manager(
      new ExtensionIconManager());
  prompt->set_icon(icon_manager->GetIcon(extension_->id()));

  return prompt;
}
