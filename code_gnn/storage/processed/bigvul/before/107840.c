bool PrintWebViewHelper::GetDefaultPrintSettings(
    WebFrame* frame,
    WebNode* node,
    ViewMsg_Print_Params* params) {
  IPC::SyncMessage* msg =
      new ViewHostMsg_GetDefaultPrintSettings(routing_id(), params);
  if (!Send(msg)) {
    NOTREACHED();
    return false;
  }
  if (params->IsEmpty()) {
    render_view_->runModalAlertDialog(
        frame,
        l10n_util::GetStringUTF16(IDS_DEFAULT_PRINTER_NOT_FOUND_WARNING));
    return false;
  }
  if (!(params->dpi && params->document_cookie)) {
    NOTREACHED();
    return false;
  }
  UpdatePrintableSizeInPrintParameters(frame, node, params);
  return true;
}
