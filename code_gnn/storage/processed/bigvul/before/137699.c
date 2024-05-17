void PrintViewManagerBase::DisconnectFromCurrentPrintJob() {
  bool result = RenderAllMissingPagesNow();

  if (print_job_.get() &&
      print_job_->document() &&
      !print_job_->document()->IsComplete()) {
    DCHECK(!result);
    TerminatePrintJob(true);
  } else {
    ReleasePrintJob();
  }
#if !defined(OS_MACOSX)
  expecting_first_page_ = true;
#endif
}
