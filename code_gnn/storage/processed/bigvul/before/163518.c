 void PushMessagingServiceImpl::SetMessageCallbackForTesting(
    const base::Closure& callback) {
  message_callback_for_testing_ = callback;
}
