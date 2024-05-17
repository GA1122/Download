void PrintJobWorker::OnFailure() {
  DCHECK(task_runner_->RunsTasksInCurrentSequence());

  scoped_refptr<PrintJobWorkerOwner> handle(owner_);

  owner_->PostTask(FROM_HERE,
                   base::Bind(&NotificationCallback, base::RetainedRef(owner_),
                              JobEventDetails::FAILED, 0,
                              base::RetainedRef(document_), nullptr));
  Cancel();

  document_ = NULL;
  page_number_ = PageNumber::npos();
}
