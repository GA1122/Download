  ChildProcessSecurityPolicyImpl::CheckOriginLockResult CheckOriginLock(
      const GURL& gurl) {
    if (origin_lock_.is_empty())
      return ChildProcessSecurityPolicyImpl::CheckOriginLockResult::NO_LOCK;

    if (origin_lock_ == gurl) {
      return ChildProcessSecurityPolicyImpl::CheckOriginLockResult::
          HAS_EQUAL_LOCK;
    }

    return ChildProcessSecurityPolicyImpl::CheckOriginLockResult::
        HAS_WRONG_LOCK;
  }
