bool CSPHandler::Parse(Extension* extension, base::string16* error) {
  const std::string key = Keys()[0];
  if (!extension->manifest()->HasPath(key)) {
    if (extension->manifest_version() >= 2) {
      std::string content_security_policy = is_platform_app_ ?
          kDefaultPlatformAppContentSecurityPolicy :
          kDefaultContentSecurityPolicy;

      CHECK_EQ(content_security_policy,
               SanitizeContentSecurityPolicy(content_security_policy,
                                             GetValidatorOptions(extension),
                                             NULL));
      extension->SetManifestData(keys::kContentSecurityPolicy,
                                 new CSPInfo(content_security_policy));
    }
    return true;
  }

  std::string content_security_policy;
  if (!extension->manifest()->GetString(key, &content_security_policy)) {
    *error = base::ASCIIToUTF16(errors::kInvalidContentSecurityPolicy);
    return false;
  }
  if (!ContentSecurityPolicyIsLegal(content_security_policy)) {
    *error = base::ASCIIToUTF16(errors::kInvalidContentSecurityPolicy);
    return false;
  }
  std::string sanitized_csp;
  if (extension->manifest_version() >= 2) {
    std::vector<InstallWarning> warnings;
    content_security_policy =
        SanitizeContentSecurityPolicy(content_security_policy,
                                      GetValidatorOptions(extension),
                                      &warnings);
    extension->AddInstallWarnings(warnings);
  }

  extension->SetManifestData(keys::kContentSecurityPolicy,
                             new CSPInfo(content_security_policy));
  return true;
}
