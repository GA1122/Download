bool PrintWebViewHelper::GetPrintSettingsFromUser(WebFrame* frame,
                                                  int expected_pages_count,
                                                  bool use_browser_overlays) {
  ViewHostMsg_ScriptedPrint_Params params;
  ViewMsg_PrintPages_Params print_settings;

  params.routing_id = routing_id();
  params.host_window_id = render_view_->host_window();
  params.cookie = (*print_pages_params_).params.document_cookie;
  params.has_selection = frame->hasSelection();
  params.expected_pages_count = expected_pages_count;
  params.use_overlays = use_browser_overlays;

  print_pages_params_.reset();
  IPC::SyncMessage* msg =
      new ViewHostMsg_ScriptedPrint(routing_id(), params, &print_settings);
  msg->EnableMessagePumping();
  if (Send(msg)) {
    print_pages_params_.reset(new ViewMsg_PrintPages_Params(print_settings));
  } else {
    NOTREACHED();
    return false;
  }
  return (print_settings.params.dpi && print_settings.params.document_cookie);
}
