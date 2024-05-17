void ContentSecurityPolicy::EnforceStrictMixedContentChecking() {
  insecure_request_policy_ |= kBlockAllMixedContent;
}
