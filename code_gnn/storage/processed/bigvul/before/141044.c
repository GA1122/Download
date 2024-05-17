void Document::EnforceSandboxFlags(SandboxFlags mask) {
  scoped_refptr<const SecurityOrigin> stand_in_origin = GetSecurityOrigin();
  bool is_potentially_trustworthy =
      stand_in_origin && stand_in_origin->IsPotentiallyTrustworthy();
  ApplySandboxFlags(mask, is_potentially_trustworthy);
}
