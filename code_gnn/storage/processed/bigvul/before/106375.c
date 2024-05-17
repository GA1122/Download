 void LoginHtmlDialog::OnDialogClosed(const std::string& json_retval) {
   is_open_ = false;
   notification_registrar_.RemoveAll();
  if (delegate_)
    delegate_->OnDialogClosed();
}
