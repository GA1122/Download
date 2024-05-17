void PushMessagingServiceImpl::SetServiceWorkerUnregisteredCallbackForTesting(
    const base::Closure& callback) {
  service_worker_unregistered_callback_for_testing_ = callback;
}
