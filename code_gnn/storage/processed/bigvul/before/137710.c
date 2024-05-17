void PrintViewManagerBase::ReleasePrintJob() {
  content::RenderFrameHost* rfh = printing_rfh_;
  printing_rfh_ = nullptr;

  if (!print_job_.get())
    return;

  if (rfh) {
    auto msg = base::MakeUnique<PrintMsg_PrintingDone>(rfh->GetRoutingID(),
                                                       printing_succeeded_);
    rfh->Send(msg.release());
  }

  registrar_.Remove(this, chrome::NOTIFICATION_PRINT_JOB_EVENT,
                    content::Source<PrintJob>(print_job_.get()));
  print_job_ = nullptr;
}
