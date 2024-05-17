void PageHandler::DidCloseJavaScriptDialog(bool success,
                                           const base::string16& user_input) {
  if (!enabled_)
    return;
  pending_dialog_.Reset();
  frontend_->JavascriptDialogClosed(success, base::UTF16ToUTF8(user_input));
}
