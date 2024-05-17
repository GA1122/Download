void PushMessagingServiceImpl::SubscribeFromWorker(
    const GURL& requesting_origin,
    int64_t service_worker_registration_id,
    const content::PushSubscriptionOptions& options,
    const RegisterCallback& register_callback) {
  PushMessagingAppIdentifier app_identifier =
      PushMessagingAppIdentifier::Generate(requesting_origin,
                                           service_worker_registration_id);

  if (push_subscription_count_ + pending_push_subscription_count_ >=
      kMaxRegistrations) {
    SubscribeEndWithError(
        register_callback,
        content::mojom::PushRegistrationStatus::LIMIT_REACHED);
    return;
  }

  blink::WebPushPermissionStatus permission_status =
      GetPermissionStatus(requesting_origin, options.user_visible_only);

  if (permission_status != blink::kWebPushPermissionStatusGranted) {
    SubscribeEndWithError(
        register_callback,
        content::mojom::PushRegistrationStatus::PERMISSION_DENIED);
    return;
  }

  DoSubscribe(app_identifier, options, register_callback,
              CONTENT_SETTING_ALLOW);
}
