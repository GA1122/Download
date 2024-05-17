void Document::EnforceSandboxFlags(SandboxFlags mask) {
  scoped_refptr<SecurityOrigin> stand_in_origin = GetSecurityOrigin();
  ApplySandboxFlags(mask);
  if (stand_in_origin && !stand_in_origin->IsUnique() &&
      GetSecurityOrigin()->IsUnique()) {
    GetSecurityOrigin()->SetUniqueOriginIsPotentiallyTrustworthy(
        stand_in_origin->IsPotentiallyTrustworthy());
    if (GetFrame())
      GetFrame()->Client()->DidUpdateToUniqueOrigin();
  }
}
