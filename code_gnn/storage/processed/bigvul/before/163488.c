void PushMessagingServiceImpl::DidGetEncryptionInfo(
    const SubscriptionInfoCallback& callback,
    const std::string& p256dh,
    const std::string& auth_secret) const {
  bool is_valid = !p256dh.empty();
  callback.Run(is_valid, std::vector<uint8_t>(p256dh.begin(), p256dh.end()),
               std::vector<uint8_t>(auth_secret.begin(), auth_secret.end()));
}
