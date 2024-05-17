void ServiceWorkerHandler::OnWorkerRegistrationUpdated(
    const std::vector<ServiceWorkerRegistrationInfo>& registrations) {
  using Registration = ServiceWorker::ServiceWorkerRegistration;
  std::unique_ptr<protocol::Array<Registration>> result =
      protocol::Array<Registration>::create();
  for (const auto& registration : registrations) {
    result->addItem(Registration::Create()
        .SetRegistrationId(
            base::Int64ToString(registration.registration_id))
        .SetScopeURL(registration.pattern.spec())
        .SetIsDeleted(registration.delete_flag ==
                      ServiceWorkerRegistrationInfo::IS_DELETED)
        .Build());
  }
  frontend_->WorkerRegistrationUpdated(std::move(result));
}
