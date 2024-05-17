void PushMessagingServiceImpl::SetUnsubscribeCallbackForTesting(
    const base::Closure& callback) {
  unsubscribe_callback_for_testing_ = callback;
}
