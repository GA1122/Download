PrintJobWorker::PrintJobWorker(PrintJobWorkerOwner* owner)
    : Thread("Printing_Worker"),
      owner_(owner) {
  DCHECK_EQ(owner_->message_loop(), MessageLoop::current());
}
