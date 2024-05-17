void PrinterQuery::StopWorker() {
  if (worker_.get()) {
    worker_->Stop();
    worker_.reset();
  }
}
