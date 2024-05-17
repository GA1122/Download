void PushMessagingServiceImpl::SubscribeFromDocument(
    const GURL& requesting_origin,
    int64_t service_worker_registration_id,
    int renderer_id,
    int render_frame_id,
    const content::PushSubscriptionOptions& options,
    bool user_gesture,
    const RegisterCallback& callback) {
  PushMessagingAppIdentifier app_identifier =
      PushMessagingAppIdentifier::Generate(requesting_origin,
                                           service_worker_registration_id);

  if (push_subscription_count_ + pending_push_subscription_count_ >=
      kMaxRegistrations) {
    SubscribeEndWithError(
        callback, content::mojom::PushRegistrationStatus::LIMIT_REACHED);
    return;
  }

  content::RenderFrameHost* render_frame_host =
      content::RenderFrameHost::FromID(renderer_id, render_frame_id);
  content::WebContents* web_contents =
      content::WebContents::FromRenderFrameHost(render_frame_host);
  if (!web_contents)
    return;

  if (!options.user_visible_only) {
    web_contents->GetMainFrame()->AddMessageToConsole(
        content::CONSOLE_MESSAGE_LEVEL_ERROR, kSilentPushUnsupportedMessage);

    SubscribeEndWithError(
        callback, content::mojom::PushRegistrationStatus::PERMISSION_DENIED);
    return;
  }

  PermissionManager::Get(profile_)->RequestPermission(
      CONTENT_SETTINGS_TYPE_PUSH_MESSAGING, web_contents->GetMainFrame(),
      requesting_origin, user_gesture,
      base::Bind(&PushMessagingServiceImpl::DoSubscribe,
                 weak_factory_.GetWeakPtr(), app_identifier, options,
                 callback));
}
