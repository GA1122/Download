  bool Match(const GURL& origin,
             int mask,
             storage::SpecialStoragePolicy* policy) {
    return remover_->DoesOriginMatchMask(mask, origin, policy);
  }
