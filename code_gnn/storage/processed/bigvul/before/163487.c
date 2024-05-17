void PushMessagingServiceImpl::DidDeleteServiceWorkerRegistration(
    const GURL& origin,
    int64_t service_worker_registration_id) {
  const PushMessagingAppIdentifier& app_identifier =
      PushMessagingAppIdentifier::FindByServiceWorker(
          profile_, origin, service_worker_registration_id);
  if (app_identifier.is_null()) {
    if (!service_worker_unregistered_callback_for_testing_.is_null())
      service_worker_unregistered_callback_for_testing_.Run();
    return;
  }
  UnsubscribeInternal(
      content::mojom::PushUnregistrationReason::SERVICE_WORKER_UNREGISTERED,
      origin, service_worker_registration_id, app_identifier.app_id(),
      std::string()  ,
      base::Bind(&UnregisterCallbackToClosure,
                 service_worker_unregistered_callback_for_testing_.is_null()
                     ? base::Bind(&base::DoNothing)
                     : service_worker_unregistered_callback_for_testing_));
}
