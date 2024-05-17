void PrintJobWorker::SpoolPage(PrintedPage* page) {
  DCHECK(task_runner_->RunsTasksInCurrentSequence());
  DCHECK_NE(page_number_, PageNumber::npos());

  owner_->PostTask(
      FROM_HERE,
      base::Bind(&NotificationCallback, base::RetainedRef(owner_),
                 JobEventDetails::NEW_PAGE, printing_context_->job_id(),
                 base::RetainedRef(document_), base::RetainedRef(page)));

  if (printing_context_->NewPage() != PrintingContext::OK) {
    OnFailure();
    return;
  }

#if defined(OS_WIN) || defined(OS_MACOSX)
  document_->RenderPrintedPage(*page, printing_context_->context());
#elif defined(OS_POSIX)
  document_->RenderPrintedPage(*page, printing_context_.get());
#endif

  if (printing_context_->PageDone() != PrintingContext::OK) {
    OnFailure();
    return;
  }

  owner_->PostTask(
      FROM_HERE,
      base::Bind(&NotificationCallback, base::RetainedRef(owner_),
                 JobEventDetails::PAGE_DONE, printing_context_->job_id(),
                 base::RetainedRef(document_), base::RetainedRef(page)));
}
