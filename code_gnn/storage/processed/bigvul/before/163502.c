void PushMessagingServiceImpl::GetSubscriptionInfo(
    const GURL& origin,
    int64_t service_worker_registration_id,
    const std::string& sender_id,
    const std::string& subscription_id,
    const SubscriptionInfoCallback& callback) {
  PushMessagingAppIdentifier app_identifier =
      PushMessagingAppIdentifier::FindByServiceWorker(
          profile_, origin, service_worker_registration_id);

  if (app_identifier.is_null()) {
    callback.Run(false  , std::vector<uint8_t>()  ,
                 std::vector<uint8_t>()  );
    return;
  }

  const std::string& app_id = app_identifier.app_id();
  base::Callback<void(bool)> validate_cb =
      base::Bind(&PushMessagingServiceImpl::DidValidateSubscription,
                 weak_factory_.GetWeakPtr(), app_id, sender_id, callback);

  if (PushMessagingAppIdentifier::UseInstanceID(app_id)) {
    GetInstanceIDDriver()->GetInstanceID(app_id)->ValidateToken(
        NormalizeSenderInfo(sender_id), kGCMScope, subscription_id,
        validate_cb);
  } else {
    GetGCMDriver()->ValidateRegistration(
        app_id, {NormalizeSenderInfo(sender_id)}, subscription_id, validate_cb);
  }
}
