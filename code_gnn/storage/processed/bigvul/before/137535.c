bool PrintWebViewHelper::GetPrintSettingsFromUser(blink::WebLocalFrame* frame,
                                                  const blink::WebNode& node,
                                                  int expected_pages_count,
                                                  bool is_scripted) {
  PrintHostMsg_ScriptedPrint_Params params;
  PrintMsg_PrintPages_Params print_settings;

  params.cookie = print_pages_params_->params.document_cookie;
  params.has_selection = frame->hasSelection();
  params.expected_pages_count = expected_pages_count;
  MarginType margin_type = DEFAULT_MARGINS;
  if (PrintingNodeOrPdfFrame(frame, node)) {
    margin_type =
        GetMarginsForPdf(frame, node, print_pages_params_->params);
  }
  params.margin_type = margin_type;
  params.is_scripted = is_scripted;

  Send(new PrintHostMsg_DidShowPrintDialog(routing_id()));

  blink::WebPrintScalingOption scaling_option =
      print_pages_params_->params.print_scaling_option;

  print_pages_params_.reset();
  IPC::SyncMessage* msg =
      new PrintHostMsg_ScriptedPrint(routing_id(), params, &print_settings);
  msg->EnableMessagePumping();
  Send(msg);
  print_settings.params.print_scaling_option = scaling_option;
  SetPrintPagesParams(print_settings);
  return (print_settings.params.dpi && print_settings.params.document_cookie);
}
