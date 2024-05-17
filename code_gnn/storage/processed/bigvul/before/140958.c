void Document::ApplyFeaturePolicyFromHeader(
    const String& feature_policy_header) {
  if (!feature_policy_header.IsEmpty())
    UseCounter::Count(*this, WebFeature::kFeaturePolicyHeader);
  Vector<String> messages;
  auto declared_policy = ParseFeaturePolicyHeader(
      feature_policy_header, GetSecurityOrigin(), &messages, this);
  for (auto& message : messages) {
    AddConsoleMessage(ConsoleMessage::Create(
        kSecurityMessageSource, mojom::ConsoleMessageLevel::kError,
        "Error with Feature-Policy header: " + message));
  }
  if (GetSandboxFlags() != kSandboxNone &&
      RuntimeEnabledFeatures::FeaturePolicyForSandboxEnabled()) {
    ApplySandboxFlagsToParsedFeaturePolicy(GetSandboxFlags(), declared_policy);
  }
  ApplyFeaturePolicy(declared_policy);
  if (frame_) {
    frame_->Client()->DidSetFramePolicyHeaders(GetSandboxFlags(),
                                               declared_policy);
  }
}
