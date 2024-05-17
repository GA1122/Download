void PushMessagingServiceImpl::DidValidateSubscription(
    const std::string& app_id,
    const std::string& sender_id,
    const SubscriptionInfoCallback& callback,
    bool is_valid) {
  if (!is_valid) {
    callback.Run(false  , std::vector<uint8_t>()  ,
                 std::vector<uint8_t>()  );
    return;
  }

  GetEncryptionInfoForAppId(
      app_id, sender_id,
      base::Bind(&PushMessagingServiceImpl::DidGetEncryptionInfo,
                 weak_factory_.GetWeakPtr(), callback));
}
