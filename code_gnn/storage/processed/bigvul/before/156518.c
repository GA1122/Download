bool ChildProcessSecurityPolicyImpl::CanAccessDataForOrigin(int child_id,
                                                            const GURL& url) {
  GURL site_url = SiteInstance::GetSiteForURL(nullptr, url);

  base::AutoLock lock(lock_);
  SecurityStateMap::iterator state = security_state_.find(child_id);
  if (state == security_state_.end()) {
    return true;
  }
  bool can_access = state->second->CanAccessDataForOrigin(site_url);
  if (!can_access) {
    base::debug::SetCrashKeyString(bad_message::GetRequestedSiteURLKey(),
                                   site_url.spec());
    base::debug::SetCrashKeyString(bad_message::GetKilledProcessOriginLockKey(),
                                   state->second->origin_lock().spec());

    static auto* requested_origin_key = base::debug::AllocateCrashKeyString(
        "requested_origin", base::debug::CrashKeySize::Size64);
    base::debug::SetCrashKeyString(requested_origin_key,
                                   url.GetOrigin().spec());
  }
  return can_access;
}
