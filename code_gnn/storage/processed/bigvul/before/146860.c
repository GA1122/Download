void Document::UpdateSecurityOrigin(RefPtr<SecurityOrigin> origin) {
  SetSecurityOrigin(std::move(origin));
  DidUpdateSecurityOrigin();
}
