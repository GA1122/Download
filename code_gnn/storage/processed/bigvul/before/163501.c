blink::WebPushPermissionStatus PushMessagingServiceImpl::GetPermissionStatus(
    const GURL& origin,
    bool user_visible) {
  if (!user_visible)
    return blink::kWebPushPermissionStatusDenied;

  return ToPushPermission(
      PermissionManager::Get(profile_)
          ->GetPermissionStatus(CONTENT_SETTINGS_TYPE_PUSH_MESSAGING, origin,
                                origin)
          .content_setting);
}
