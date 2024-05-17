bool FrameFetchContext::AllowImage(bool images_enabled, const KURL& url) const {
  if (IsDetached())
    return true;

  return GetContentSettingsClient()->AllowImage(images_enabled, url);
}
