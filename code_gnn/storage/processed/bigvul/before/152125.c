void ServiceWorkerContextCore::DeleteAndStartOver(StatusCallback callback) {
  job_coordinator_->AbortAll();
  storage_->DeleteAndStartOver(std::move(callback));
}
