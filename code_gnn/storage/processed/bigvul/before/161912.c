void PrintRenderFrameHelper::GetPrintSettingsFromUser(
    blink::WebLocalFrame* frame,
    const blink::WebNode& node,
    int expected_pages_count,
    bool is_scripted,
    PrintMsg_PrintPages_Params* print_settings) {
  PrintHostMsg_ScriptedPrint_Params params;
  params.cookie = print_pages_params_->params.document_cookie;
  params.has_selection = frame->HasSelection();
  params.expected_pages_count = expected_pages_count;
  MarginType margin_type = DEFAULT_MARGINS;
  if (PrintingNodeOrPdfFrame(frame, node))
    margin_type = GetMarginsForPdf(frame, node, print_pages_params_->params);
  params.margin_type = margin_type;
  params.is_scripted = is_scripted;
  params.is_modifiable = !PrintingNodeOrPdfFrame(frame, node);

  Send(new PrintHostMsg_DidShowPrintDialog(routing_id()));

  print_pages_params_.reset();

  auto msg = base::MakeUnique<PrintHostMsg_ScriptedPrint>(routing_id(), params,
                                                          print_settings);
  msg->EnableMessagePumping();
  Send(msg.release());
}
