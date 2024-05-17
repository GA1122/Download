void ExtensionInstallPrompt::ConfirmStandaloneInstall(
    Delegate* delegate,
    const Extension* extension,
    SkBitmap* icon,
    scoped_refptr<Prompt> prompt) {
  DCHECK(ui_loop_ == base::MessageLoop::current());
  extension_ = extension;
  delegate_ = delegate;
  prompt_ = prompt;

  SetIcon(icon);
  ShowConfirmation();
}
