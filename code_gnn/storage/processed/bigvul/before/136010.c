ChildProcessSecurityPolicyImpl::ChildProcessSecurityPolicyImpl() {
  RegisterWebSafeScheme(url::kHttpScheme);
  RegisterWebSafeScheme(url::kHttpsScheme);
  RegisterWebSafeScheme(url::kFtpScheme);
  RegisterWebSafeScheme(url::kDataScheme);
  RegisterWebSafeScheme("feed");
  RegisterWebSafeScheme(url::kBlobScheme);
  RegisterWebSafeScheme(url::kFileSystemScheme);

  RegisterPseudoScheme(url::kAboutScheme);
  RegisterPseudoScheme(url::kJavaScriptScheme);
  RegisterPseudoScheme(kViewSourceScheme);
}
