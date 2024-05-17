String MixedContentTypeForContextType(WebMixedContentContextType context_type) {
  switch (context_type) {
    case WebMixedContentContextType::kNotMixedContent:
      return protocol::Security::MixedContentTypeEnum::None;
    case WebMixedContentContextType::kBlockable:
      return protocol::Security::MixedContentTypeEnum::Blockable;
    case WebMixedContentContextType::kOptionallyBlockable:
    case WebMixedContentContextType::kShouldBeBlockable:
      return protocol::Security::MixedContentTypeEnum::OptionallyBlockable;
  }

  return protocol::Security::MixedContentTypeEnum::None;
}
