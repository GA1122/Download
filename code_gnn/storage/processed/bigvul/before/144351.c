void ExtensionInstallPrompt::ConfirmExternalInstall(
    Delegate* delegate,
    const Extension* extension,
    const ShowDialogCallback& show_dialog_callback,
    scoped_refptr<Prompt> prompt) {
  DCHECK(ui_loop_ == base::MessageLoop::current());
  extension_ = extension;
  delegate_ = delegate;
  prompt_ = prompt;
  show_dialog_callback_ = show_dialog_callback;

  LoadImageIfNeeded();
}
