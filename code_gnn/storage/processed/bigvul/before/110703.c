 void AppModalDialog::ShowModalDialog() {
  web_contents_->GetDelegate()->ActivateContents(web_contents_);
  CreateAndShowDialog();

  content::NotificationService::current()->Notify(
      chrome::NOTIFICATION_APP_MODAL_DIALOG_SHOWN,
      content::Source<AppModalDialog>(this),
      content::NotificationService::NoDetails());
}
