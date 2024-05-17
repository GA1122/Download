void Document::ApplyFeaturePolicyFromHeader(
    const String& feature_policy_header) {
  if (!feature_policy_header.IsEmpty())
    UseCounter::Count(*this, WebFeature::kFeaturePolicyHeader);
  Vector<String> messages;
  const ParsedFeaturePolicy& declared_policy = ParseFeaturePolicyHeader(
      feature_policy_header, GetSecurityOrigin(), &messages);
  for (auto& message : messages) {
    AddConsoleMessage(
        ConsoleMessage::Create(kSecurityMessageSource, kErrorMessageLevel,
                               "Error with Feature-Policy header: " + message));
  }
  ApplyFeaturePolicy(declared_policy);
  if (frame_) {
    frame_->Client()->DidSetFramePolicyHeaders(GetSandboxFlags(),
                                               declared_policy);
  }
}
