void PrintJobWorker::OnDocumentDone() {
  DCHECK_EQ(message_loop(), MessageLoop::current());
  DCHECK_EQ(page_number_, PageNumber::npos());
  DCHECK(document_.get());
#if !defined(OS_MACOSX)
  DCHECK(printing_context_.context());
#endif

  if (printing_context_.DocumentDone() != PrintingContext::OK) {
    OnFailure();
    return;
  }

  NotificationTask* task = new NotificationTask();
  task->Init(owner_,
             JobEventDetails::DOC_DONE,
             document_.get(),
             NULL);
  owner_->message_loop()->PostTask(FROM_HERE, task);

  document_ = NULL;
}
