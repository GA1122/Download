std::string IDNSpoofChecker::GetSimilarTopDomain(base::StringPiece16 hostname) {
  DCHECK(!hostname.empty());
  for (const std::string& skeleton : GetSkeletons(hostname)) {
    DCHECK(!skeleton.empty());
    std::string matching_top_domain = LookupMatchInTopDomains(skeleton);
    if (!matching_top_domain.empty())
      return matching_top_domain;
  }
  return std::string();
}
