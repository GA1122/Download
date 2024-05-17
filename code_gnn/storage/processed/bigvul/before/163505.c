bool PushMessagingServiceImpl::IsPermissionSet(const GURL& origin) {
  return GetPermissionStatus(origin, true  ) ==
         blink::kWebPushPermissionStatusGranted;
}
