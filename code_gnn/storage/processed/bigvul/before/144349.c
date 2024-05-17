bool AutoConfirmPrompt(ExtensionInstallPrompt::Delegate* delegate) {
  switch (extensions::ScopedTestDialogAutoConfirm::GetAutoConfirmValue()) {
    case extensions::ScopedTestDialogAutoConfirm::NONE:
      return false;
    case extensions::ScopedTestDialogAutoConfirm::ACCEPT:
      base::MessageLoop::current()->PostTask(
          FROM_HERE,
          base::Bind(&ExtensionInstallPrompt::Delegate::InstallUIProceed,
                     base::Unretained(delegate)));
      return true;
    case extensions::ScopedTestDialogAutoConfirm::CANCEL:
      base::ThreadTaskRunnerHandle::Get()->PostTask(
          FROM_HERE,
          base::Bind(&ExtensionInstallPrompt::Delegate::InstallUIAbort,
                     base::Unretained(delegate),
                     true));
      return true;
  }

  NOTREACHED();
  return false;
}
