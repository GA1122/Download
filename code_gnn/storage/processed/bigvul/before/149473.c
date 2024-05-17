void ContentSecurityPolicy::reportMixedContent(const KURL& mixedURL,
                                               RedirectStatus redirectStatus) {
  for (const auto& policy : m_policies)
    policy->reportMixedContent(mixedURL, redirectStatus);
}
