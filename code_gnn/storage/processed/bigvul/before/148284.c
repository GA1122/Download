PrintJobWorker::~PrintJobWorker() {
  DCHECK(owner_->RunsTasksInCurrentSequence());
  Stop();
}
