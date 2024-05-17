void PrintJobWorker::OnDocumentDone() {
  DCHECK(task_runner_->RunsTasksInCurrentSequence());
  DCHECK_EQ(page_number_, PageNumber::npos());
  DCHECK(document_.get());

  int job_id = printing_context_->job_id();
  if (printing_context_->DocumentDone() != PrintingContext::OK) {
    OnFailure();
    return;
  }

  owner_->PostTask(FROM_HERE,
                   base::Bind(&NotificationCallback, base::RetainedRef(owner_),
                              JobEventDetails::DOC_DONE, job_id,
                              base::RetainedRef(document_), nullptr));

  document_ = NULL;
}
