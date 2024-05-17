void PushMessagingServiceImpl::DidClearPushSubscriptionId(
    content::mojom::PushUnregistrationReason reason,
    const std::string& app_id,
    const std::string& sender_id,
    const UnregisterCallback& callback) {
  if (app_id.empty()) {
    callback.Run(
        content::mojom::PushUnregistrationStatus::SUCCESS_WAS_NOT_REGISTERED);
    return;
  }

  PushMessagingAppIdentifier app_identifier =
      PushMessagingAppIdentifier::FindByAppId(profile_, app_id);
  bool was_subscribed = !app_identifier.is_null();
  if (was_subscribed)
    app_identifier.DeleteFromPrefs(profile_);

  callback.Run(
      was_subscribed
          ? content::mojom::PushUnregistrationStatus::SUCCESS_UNREGISTERED
          : content::mojom::PushUnregistrationStatus::
                SUCCESS_WAS_NOT_REGISTERED);

  if (PushMessagingAppIdentifier::UseInstanceID(app_id)) {
    GetInstanceIDDriver()->GetInstanceID(app_id)->DeleteID(
        base::Bind(&PushMessagingServiceImpl::DidDeleteID,
                   weak_factory_.GetWeakPtr(), app_id, was_subscribed));

  } else {
    auto unregister_callback =
        base::Bind(&PushMessagingServiceImpl::DidUnregister,
                   weak_factory_.GetWeakPtr(), was_subscribed);
#if defined(OS_ANDROID)
    if (sender_id.empty()) {
      unregister_callback.Run(gcm::GCMClient::INVALID_PARAMETER);
    } else {
      GetGCMDriver()->UnregisterWithSenderId(
          app_id, NormalizeSenderInfo(sender_id), unregister_callback);
    }
#else
    GetGCMDriver()->Unregister(app_id, unregister_callback);
#endif
  }
}
