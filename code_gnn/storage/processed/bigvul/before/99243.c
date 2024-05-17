void PrintJobWorker::OnFailure() {
  DCHECK_EQ(message_loop(), MessageLoop::current());

  scoped_refptr<PrintJobWorkerOwner> handle(owner_);

  NotificationTask* task = new NotificationTask();
  task->Init(owner_,
             JobEventDetails::FAILED,
             document_.get(),
             NULL);
  owner_->message_loop()->PostTask(FROM_HERE, task);
  Cancel();

  document_ = NULL;
  page_number_ = PageNumber::npos();
}
