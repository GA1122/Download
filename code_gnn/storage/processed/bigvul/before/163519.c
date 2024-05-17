void PushMessagingServiceImpl::SetServiceWorkerDatabaseWipedCallbackForTesting(
    const base::Closure& callback) {
  service_worker_database_wiped_callback_for_testing_ = callback;
}
