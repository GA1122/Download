void PushMessagingServiceImpl::OnContentSettingChanged(
    const ContentSettingsPattern& primary_pattern,
    const ContentSettingsPattern& secondary_pattern,
    ContentSettingsType content_type,
    std::string resource_identifier) {
  if (content_type != CONTENT_SETTINGS_TYPE_NOTIFICATIONS)
    return;

  std::vector<PushMessagingAppIdentifier> all_app_identifiers =
      PushMessagingAppIdentifier::GetAll(profile_);

  base::Closure barrier_closure = base::BarrierClosure(
      all_app_identifiers.size(),
      content_setting_changed_callback_for_testing_.is_null()
          ? base::Bind(&base::DoNothing)
          : content_setting_changed_callback_for_testing_);

  for (const PushMessagingAppIdentifier& app_identifier : all_app_identifiers) {
    if (primary_pattern.IsValid() &&
        !primary_pattern.Matches(app_identifier.origin())) {
      barrier_closure.Run();
      continue;
    }

    if (IsPermissionSet(app_identifier.origin())) {
      barrier_closure.Run();
      continue;
    }

    bool need_sender_id = false;
#if defined(OS_ANDROID)
    need_sender_id =
        !PushMessagingAppIdentifier::UseInstanceID(app_identifier.app_id());
#endif
    if (need_sender_id) {
      GetSenderId(
          profile_, app_identifier.origin(),
          app_identifier.service_worker_registration_id(),
          base::Bind(
              &PushMessagingServiceImpl::UnsubscribeBecausePermissionRevoked,
              weak_factory_.GetWeakPtr(), app_identifier,
              base::Bind(&UnregisterCallbackToClosure, barrier_closure)));
    } else {
      UnsubscribeInternal(
          content::mojom::PushUnregistrationReason::PERMISSION_REVOKED,
          app_identifier.origin(),
          app_identifier.service_worker_registration_id(),
          app_identifier.app_id(), std::string()  ,
          base::Bind(&UnregisterCallbackToClosure, barrier_closure));
    }
  }
}
