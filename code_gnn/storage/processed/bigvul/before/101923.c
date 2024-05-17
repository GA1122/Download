void PrintingMessageFilter::OnGetDefaultPrintSettings(IPC::Message* reply_msg) {
  scoped_refptr<printing::PrinterQuery> printer_query;
  if (!print_job_manager_->printing_enabled()) {
    OnGetDefaultPrintSettingsReply(printer_query, reply_msg);
    return;
  }

  print_job_manager_->PopPrinterQuery(0, &printer_query);
  if (!printer_query.get()) {
    printer_query = new printing::PrinterQuery;
  }

  CancelableTask* task = NewRunnableMethod(
      this,
      &PrintingMessageFilter::OnGetDefaultPrintSettingsReply,
      printer_query,
      reply_msg);
  printer_query->GetSettings(printing::PrinterQuery::DEFAULTS,
                             NULL,
                             0,
                             false,
                             true,
                             task);
}
