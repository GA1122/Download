bool Extension::LoadSandboxedPages(string16* error) {
  if (!manifest_->HasPath(keys::kSandboxedPages))
    return true;

  ListValue* list_value = NULL;
  if (!manifest_->GetList(keys::kSandboxedPages, &list_value)) {
    *error = ASCIIToUTF16(errors::kInvalidSandboxedPagesList);
    return false;
  }
  for (size_t i = 0; i < list_value->GetSize(); ++i) {
    std::string relative_path;
    if (!list_value->GetString(i, &relative_path)) {
      *error = ErrorUtils::FormatErrorMessageUTF16(
          errors::kInvalidSandboxedPage, base::IntToString(i));
      return false;
    }
    URLPattern pattern(URLPattern::SCHEME_EXTENSION);
    if (pattern.Parse(extension_url_.spec()) != URLPattern::PARSE_SUCCESS) {
      *error = ErrorUtils::FormatErrorMessageUTF16(
          errors::kInvalidURLPatternError, extension_url_.spec());
      return false;
    }
    while (relative_path[0] == '/')
      relative_path = relative_path.substr(1, relative_path.length() - 1);
    pattern.SetPath(pattern.path() + relative_path);
    sandboxed_pages_.AddPattern(pattern);
  }

  if (manifest_->HasPath(keys::kSandboxedPagesCSP)) {
    if (!manifest_->GetString(
        keys::kSandboxedPagesCSP, &sandboxed_pages_content_security_policy_)) {
      *error = ASCIIToUTF16(errors::kInvalidSandboxedPagesCSP);
      return false;
    }

    if (!ContentSecurityPolicyIsLegal(
            sandboxed_pages_content_security_policy_) ||
        !ContentSecurityPolicyIsSandboxed(
            sandboxed_pages_content_security_policy_, GetType())) {
      *error = ASCIIToUTF16(errors::kInvalidSandboxedPagesCSP);
      return false;
    }
  } else {
    sandboxed_pages_content_security_policy_ =
        kDefaultSandboxedPageContentSecurityPolicy;
    CHECK(ContentSecurityPolicyIsSandboxed(
        sandboxed_pages_content_security_policy_, GetType()));
  }

  return true;
}
