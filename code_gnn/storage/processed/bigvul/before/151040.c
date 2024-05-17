void DevToolsUIBindings::ShowDevToolsConfirmInfoBar(
    const base::string16& message,
    const InfoBarCallback& callback) {
  if (!delegate_->GetInfoBarService()) {
    callback.Run(false);
    return;
  }
  std::unique_ptr<DevToolsConfirmInfoBarDelegate> delegate(
      new DevToolsConfirmInfoBarDelegate(callback, message));
   GlobalConfirmInfoBar::Show(std::move(delegate));
 }
