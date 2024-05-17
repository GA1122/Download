void PushMessagingServiceImpl::UnsubscribeBecausePermissionRevoked(
    const PushMessagingAppIdentifier& app_identifier,
    const UnregisterCallback& callback,
    const std::string& sender_id,
    bool success,
    bool not_found) {
  UnsubscribeInternal(
      content::mojom::PushUnregistrationReason::PERMISSION_REVOKED,
      app_identifier.origin(), app_identifier.service_worker_registration_id(),
      app_identifier.app_id(), sender_id, callback);
}
