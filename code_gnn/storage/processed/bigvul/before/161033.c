TopDomainEntry IDNSpoofChecker::GetSimilarTopDomain(
    base::StringPiece16 hostname) {
  DCHECK(!hostname.empty());
  for (const std::string& skeleton : GetSkeletons(hostname)) {
    DCHECK(!skeleton.empty());
    TopDomainEntry matching_top_domain = LookupSkeletonInTopDomains(skeleton);
    if (!matching_top_domain.domain.empty()) {
      return matching_top_domain;
    }
  }
  return TopDomainEntry();
}
