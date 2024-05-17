void PrintingMessageFilter::OnGetDefaultPrintSettings(IPC::Message* reply_msg) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  scoped_refptr<printing::PrinterQuery> printer_query;
  if (!profile_io_data_->printing_enabled()->GetValue()) {
    OnGetDefaultPrintSettingsReply(printer_query, reply_msg);
    return;
  }
  print_job_manager_->PopPrinterQuery(0, &printer_query);
  if (!printer_query.get()) {
    printer_query = new printing::PrinterQuery;
    printer_query->SetWorkerDestination(print_job_manager_->destination());
  }

  GetPrintSettingsForRenderViewParams params;
  params.ask_user_for_settings = printing::PrinterQuery::DEFAULTS;
  params.expected_page_count = 0;
  params.has_selection = false;
  params.margin_type = printing::DEFAULT_MARGINS;
  BrowserThread::PostTask(
      BrowserThread::UI, FROM_HERE,
      base::Bind(&PrintingMessageFilter::GetPrintSettingsForRenderView, this,
          reply_msg->routing_id(), params,
          base::Bind(&PrintingMessageFilter::OnGetDefaultPrintSettingsReply,
              this, printer_query, reply_msg),
          printer_query));
}
