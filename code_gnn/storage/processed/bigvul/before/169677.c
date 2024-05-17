void SiteInstanceImpl::LockToOriginIfNeeded() {
  DCHECK(HasSite());

  process_->SetIsUsed();

  ChildProcessSecurityPolicyImpl* policy =
      ChildProcessSecurityPolicyImpl::GetInstance();
  auto lock_state = policy->CheckOriginLock(process_->GetID(), lock_url());
  if (ShouldLockToOrigin(GetBrowserContext(), site_)) {
    CHECK(!process_->IsForGuestsOnly());

    switch (lock_state) {
      case CheckOriginLockResult::NO_LOCK: {
        TRACE_EVENT2("navigation", "SiteInstanceImpl::LockToOrigin", "site id",
                     id_, "lock", lock_url().possibly_invalid_spec());
        process_->LockToOrigin(lock_url());
        break;
      }
      case CheckOriginLockResult::HAS_WRONG_LOCK:
        base::debug::SetCrashKeyString(bad_message::GetRequestedSiteURLKey(),
                                       site_.spec());
        base::debug::SetCrashKeyString(
            bad_message::GetKilledProcessOriginLockKey(),
            policy->GetOriginLock(process_->GetID()).spec());
        CHECK(false) << "Trying to lock a process to " << lock_url()
                     << " but the process is already locked to "
                     << policy->GetOriginLock(process_->GetID());
        break;
      case CheckOriginLockResult::HAS_EQUAL_LOCK:
        break;
      default:
        NOTREACHED();
    }
  } else {
    if (lock_state != CheckOriginLockResult::NO_LOCK) {
      base::debug::SetCrashKeyString(bad_message::GetRequestedSiteURLKey(),
                                     site_.spec());
      base::debug::SetCrashKeyString(
          bad_message::GetKilledProcessOriginLockKey(),
          policy->GetOriginLock(process_->GetID()).spec());
      CHECK(false) << "Trying to commit non-isolated site " << site_
                   << " in process locked to "
                   << policy->GetOriginLock(process_->GetID());
    }
  }
}
