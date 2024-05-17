void PushMessagingServiceImpl::OnStoreReset() {
  for (const auto& identifier : PushMessagingAppIdentifier::GetAll(profile_)) {
    RecordUnsubscribeReason(
        content::mojom::PushUnregistrationReason::GCM_STORE_RESET);
    ClearPushSubscriptionId(profile_, identifier.origin(),
                            identifier.service_worker_registration_id(),
                            base::Bind(&base::DoNothing));
  }
  PushMessagingAppIdentifier::DeleteAllFromPrefs(profile_);
}
