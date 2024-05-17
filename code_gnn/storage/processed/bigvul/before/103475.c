void SimpleExtensionLoadPrompt::InstallUIProceed() {
  if (extension_service_.get())
    extension_service_->OnExtensionInstalled(
        extension_, false, 0);   
  delete this;
}
