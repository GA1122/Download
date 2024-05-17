void PrintingMessageFilter::OnUpdatePrintSettings(
    int document_cookie, const DictionaryValue& job_settings,
    IPC::Message* reply_msg) {
  scoped_refptr<printing::PrinterQuery> printer_query;
  if (!profile_io_data_->printing_enabled()->GetValue()) {
    OnUpdatePrintSettingsReply(printer_query, reply_msg);
    return;
  }

  print_job_manager_->PopPrinterQuery(document_cookie, &printer_query);
  if (!printer_query.get()) {
    printer_query = new printing::PrinterQuery;
    printer_query->SetWorkerDestination(print_job_manager_->destination());
  }
  printer_query->SetSettings(
      job_settings,
      base::Bind(&PrintingMessageFilter::OnUpdatePrintSettingsReply, this,
                 printer_query, reply_msg));
}
