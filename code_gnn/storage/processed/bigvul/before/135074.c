void AppCacheHost::NotifyMainResourceBlocked(const GURL& manifest_url) {
  main_resource_blocked_ = true;
  blocked_manifest_url_ = manifest_url;
}
