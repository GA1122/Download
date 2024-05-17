void PushMessagingServiceImpl::DeliverMessageCallback(
    const std::string& app_id,
    const GURL& requesting_origin,
    int64_t service_worker_registration_id,
    const gcm::IncomingMessage& message,
    const base::Closure& message_handled_closure,
    content::mojom::PushDeliveryStatus status) {
  DCHECK_GE(in_flight_message_deliveries_.count(app_id), 1u);

  RecordDeliveryStatus(status);

  base::Closure completion_closure =
      base::Bind(&PushMessagingServiceImpl::DidHandleMessage,
                 weak_factory_.GetWeakPtr(), app_id, message_handled_closure);
  base::ScopedClosureRunner completion_closure_runner(completion_closure);

  content::mojom::PushUnregistrationReason unsubscribe_reason =
      content::mojom::PushUnregistrationReason::UNKNOWN;

  switch (status) {
    case content::mojom::PushDeliveryStatus::SUCCESS:
    case content::mojom::PushDeliveryStatus::EVENT_WAITUNTIL_REJECTED:
    case content::mojom::PushDeliveryStatus::TIMEOUT:
      if (in_flight_message_deliveries_.count(app_id) == 1 &&
          !base::CommandLine::ForCurrentProcess()->HasSwitch(
              switches::kAllowSilentPush)) {
        notification_manager_.EnforceUserVisibleOnlyRequirements(
            requesting_origin, service_worker_registration_id,
            base::AdaptCallbackForRepeating(
                completion_closure_runner.Release()));
      }
      break;
    case content::mojom::PushDeliveryStatus::SERVICE_WORKER_ERROR:
      break;
    case content::mojom::PushDeliveryStatus::UNKNOWN_APP_ID:
      unsubscribe_reason =
          content::mojom::PushUnregistrationReason::DELIVERY_UNKNOWN_APP_ID;
      break;
    case content::mojom::PushDeliveryStatus::PERMISSION_DENIED:
      unsubscribe_reason =
          content::mojom::PushUnregistrationReason::DELIVERY_PERMISSION_DENIED;
      break;
    case content::mojom::PushDeliveryStatus::NO_SERVICE_WORKER:
      unsubscribe_reason =
          content::mojom::PushUnregistrationReason::DELIVERY_NO_SERVICE_WORKER;
      break;
  }

  if (unsubscribe_reason != content::mojom::PushUnregistrationReason::UNKNOWN) {
    PushMessagingAppIdentifier app_identifier =
        PushMessagingAppIdentifier::FindByAppId(profile_, app_id);
    UnsubscribeInternal(
        unsubscribe_reason,
        app_identifier.is_null() ? GURL::EmptyGURL() : app_identifier.origin(),
        app_identifier.is_null()
            ? -1  
            : app_identifier.service_worker_registration_id(),
        app_id, message.sender_id,
        base::Bind(&UnregisterCallbackToClosure,
                   base::AdaptCallbackForRepeating(
                       completion_closure_runner.Release())));
  }
}
