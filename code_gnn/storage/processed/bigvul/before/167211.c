base::WeakPtr<GlobalConfirmInfoBar> GlobalConfirmInfoBar::Show(
    std::unique_ptr<ConfirmInfoBarDelegate> delegate) {
  GlobalConfirmInfoBar* info_bar =
      new GlobalConfirmInfoBar(std::move(delegate));
  return info_bar->weak_factory_.GetWeakPtr();
}
