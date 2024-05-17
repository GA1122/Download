 const std::string GetVersionRunningStatusString(
     EmbeddedWorkerStatus running_status) {
   switch (running_status) {
    case EmbeddedWorkerStatus::STOPPED:
      return ServiceWorker::ServiceWorkerVersionRunningStatusEnum::Stopped;
    case EmbeddedWorkerStatus::STARTING:
      return ServiceWorker::ServiceWorkerVersionRunningStatusEnum::Starting;
    case EmbeddedWorkerStatus::RUNNING:
      return ServiceWorker::ServiceWorkerVersionRunningStatusEnum::Running;
    case EmbeddedWorkerStatus::STOPPING:
      return ServiceWorker::ServiceWorkerVersionRunningStatusEnum::Stopping;
    default:
      NOTREACHED();
  }
  return std::string();
}
