void ExtensionInstallPrompt::ReviewPermissions(
    Delegate* delegate,
    const Extension* extension,
    const std::vector<base::FilePath>& retained_file_paths,
    const std::vector<base::string16>& retained_device_messages) {
  DCHECK(ui_loop_ == base::MessageLoop::current());
  extension_ = extension;
  prompt_ = new Prompt(POST_INSTALL_PERMISSIONS_PROMPT);
  prompt_->set_retained_files(retained_file_paths);
  prompt_->set_retained_device_messages(retained_device_messages);
  delegate_ = delegate;

  LoadImageIfNeeded();
}
