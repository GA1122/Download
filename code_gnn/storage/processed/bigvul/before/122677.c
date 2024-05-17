bool Extension::LoadContentSecurityPolicy(string16* error) {
  const std::string& key = is_platform_app() ?
      keys::kPlatformAppContentSecurityPolicy : keys::kContentSecurityPolicy;

  if (manifest_->HasPath(key)) {
    std::string content_security_policy;
    if (!manifest_->GetString(key, &content_security_policy)) {
      *error = ASCIIToUTF16(errors::kInvalidContentSecurityPolicy);
      return false;
    }
    if (!ContentSecurityPolicyIsLegal(content_security_policy)) {
      *error = ASCIIToUTF16(errors::kInvalidContentSecurityPolicy);
      return false;
    }
    if (manifest_version_ >= 2 &&
        !ContentSecurityPolicyIsSecure(content_security_policy, GetType())) {
      *error = ASCIIToUTF16(errors::kInsecureContentSecurityPolicy);
      return false;
    }

    content_security_policy_ = content_security_policy;
  } else if (manifest_version_ >= 2) {
    content_security_policy_ = is_platform_app() ?
        kDefaultPlatformAppContentSecurityPolicy :
        kDefaultContentSecurityPolicy;
    CHECK(ContentSecurityPolicyIsSecure(content_security_policy_, GetType()));
  }
  return true;
}
