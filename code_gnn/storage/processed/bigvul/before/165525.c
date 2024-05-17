bool ContentSecurityPolicy::IsActive() const {
  return !policies_.IsEmpty();
}
