bool Document::IsSecureContextImpl() const {
  if (!GetSecurityOrigin()->IsPotentiallyTrustworthy())
    return false;

  if (SchemeRegistry::SchemeShouldBypassSecureContextCheck(
          GetSecurityOrigin()->Protocol()))
    return true;

  if (!frame_)
    return true;
  Frame* parent = frame_->Tree().Parent();
  while (parent) {
    if (!parent->GetSecurityContext()
             ->GetSecurityOrigin()
             ->IsPotentiallyTrustworthy())
      return false;
    parent = parent->Tree().Parent();
  }
  return true;
}
