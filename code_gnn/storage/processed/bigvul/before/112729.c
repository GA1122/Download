void PrintingMessageFilter::OnScriptedPrint(
    const PrintHostMsg_ScriptedPrint_Params& params,
    IPC::Message* reply_msg) {
  scoped_refptr<printing::PrinterQuery> printer_query;
  print_job_manager_->PopPrinterQuery(params.cookie, &printer_query);
  if (!printer_query.get()) {
    printer_query = new printing::PrinterQuery;
    printer_query->SetWorkerDestination(print_job_manager_->destination());
  }
  GetPrintSettingsForRenderViewParams settings_params;
  settings_params.ask_user_for_settings = printing::PrinterQuery::ASK_USER;
  settings_params.expected_page_count = params.expected_pages_count;
  settings_params.has_selection = params.has_selection;
  settings_params.margin_type = params.margin_type;

  BrowserThread::PostTask(
      BrowserThread::UI, FROM_HERE,
      base::Bind(&PrintingMessageFilter::GetPrintSettingsForRenderView, this,
                 reply_msg->routing_id(), settings_params,
                 base::Bind(&PrintingMessageFilter::OnScriptedPrintReply, this,
                            printer_query, reply_msg),
                 printer_query));
}
