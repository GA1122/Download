PrintJobWorker::~PrintJobWorker() {
  DCHECK_EQ(owner_->message_loop(), MessageLoop::current());
}
