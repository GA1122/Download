void PushMessagingServiceImpl::DidSubscribe(
    const PushMessagingAppIdentifier& app_identifier,
    const std::string& sender_id,
    const RegisterCallback& callback,
    const std::string& subscription_id,
    InstanceID::Result result) {
  DecreasePushSubscriptionCount(1, true  );

  content::mojom::PushRegistrationStatus status =
      content::mojom::PushRegistrationStatus::SERVICE_ERROR;

  switch (result) {
    case InstanceID::SUCCESS:
      GetEncryptionInfoForAppId(
          app_identifier.app_id(), sender_id,
          base::Bind(&PushMessagingServiceImpl::DidSubscribeWithEncryptionInfo,
                     weak_factory_.GetWeakPtr(), app_identifier, callback,
                     subscription_id));
      return;
    case InstanceID::INVALID_PARAMETER:
    case InstanceID::DISABLED:
    case InstanceID::ASYNC_OPERATION_PENDING:
    case InstanceID::SERVER_ERROR:
    case InstanceID::UNKNOWN_ERROR:
      DLOG(ERROR) << "Push messaging subscription failed; InstanceID::Result = "
                  << result;
      status = content::mojom::PushRegistrationStatus::SERVICE_ERROR;
      break;
    case InstanceID::NETWORK_ERROR:
      status = content::mojom::PushRegistrationStatus::NETWORK_ERROR;
      break;
  }

  SubscribeEndWithError(callback, status);
}
