std::string Extension::GetResourceContentSecurityPolicy(
    const std::string& relative_path) const {
  return IsSandboxedPage(relative_path) ?
      sandboxed_pages_content_security_policy_ : content_security_policy();
}
