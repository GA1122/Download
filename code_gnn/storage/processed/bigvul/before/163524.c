void PushMessagingServiceImpl::SubscribeEnd(
    const RegisterCallback& callback,
    const std::string& subscription_id,
    const std::vector<uint8_t>& p256dh,
    const std::vector<uint8_t>& auth,
    content::mojom::PushRegistrationStatus status) {
  callback.Run(subscription_id, p256dh, auth, status);
}
