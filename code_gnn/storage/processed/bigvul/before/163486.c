void PushMessagingServiceImpl::DidDeleteServiceWorkerDatabase() {
  std::vector<PushMessagingAppIdentifier> app_identifiers =
      PushMessagingAppIdentifier::GetAll(profile_);

  base::RepeatingClosure completed_closure = base::BarrierClosure(
      app_identifiers.size(),
      service_worker_database_wiped_callback_for_testing_.is_null()
          ? base::Bind(&base::DoNothing)
          : service_worker_database_wiped_callback_for_testing_);

  for (const PushMessagingAppIdentifier& app_identifier : app_identifiers) {
    UnsubscribeInternal(
        content::mojom::PushUnregistrationReason::SERVICE_WORKER_DATABASE_WIPED,
        app_identifier.origin(),
        app_identifier.service_worker_registration_id(),
        app_identifier.app_id(), std::string()  ,
        base::Bind(&UnregisterCallbackToClosure, completed_closure));
  }
}
