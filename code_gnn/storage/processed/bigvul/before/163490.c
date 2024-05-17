void PushMessagingServiceImpl::DidSubscribeWithEncryptionInfo(
    const PushMessagingAppIdentifier& app_identifier,
    const RegisterCallback& callback,
    const std::string& subscription_id,
    const std::string& p256dh,
    const std::string& auth_secret) {
  if (p256dh.empty()) {
    SubscribeEndWithError(
        callback,
        content::mojom::PushRegistrationStatus::PUBLIC_KEY_UNAVAILABLE);
    return;
  }

  app_identifier.PersistToPrefs(profile_);

  IncreasePushSubscriptionCount(1, false  );

  SubscribeEnd(
      callback, subscription_id,
      std::vector<uint8_t>(p256dh.begin(), p256dh.end()),
      std::vector<uint8_t>(auth_secret.begin(), auth_secret.end()),
      content::mojom::PushRegistrationStatus::SUCCESS_FROM_PUSH_SERVICE);
}
