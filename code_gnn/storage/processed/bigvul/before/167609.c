void SiteInstanceImpl::LockToOriginIfNeeded() {
  DCHECK(HasSite());

  process_->SetIsUsed();

  ChildProcessSecurityPolicyImpl* policy =
      ChildProcessSecurityPolicyImpl::GetInstance();
  auto lock_state = policy->CheckOriginLock(process_->GetID(), site_);
  if (ShouldLockToOrigin(GetBrowserContext(), process_, site_)) {
    CHECK(!process_->IsForGuestsOnly());

    switch (lock_state) {
      case CheckOriginLockResult::NO_LOCK: {
        process_->LockToOrigin(site_);
        break;
      }
      case CheckOriginLockResult::HAS_WRONG_LOCK:
        base::debug::SetCrashKeyString(bad_message::GetRequestedSiteURLKey(),
                                       site_.spec());
        base::debug::SetCrashKeyString(
            bad_message::GetKilledProcessOriginLockKey(),
            policy->GetOriginLock(process_->GetID()).spec());
        CHECK(false) << "Trying to lock a process to " << site_
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
