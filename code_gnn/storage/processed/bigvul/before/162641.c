void HeadlessPrintManager::OnGetDefaultPrintSettings(IPC::Message* reply_msg) {
  PrintHostMsg_GetDefaultPrintSettings::WriteReplyParams(reply_msg,
                                                         print_params_->params);
  printing_rfh_->Send(reply_msg);
}
