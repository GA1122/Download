std::string MixedContentTypeToProtocolMixedContentType(
    blink::WebMixedContentContextType mixed_content_type) {
  switch (mixed_content_type) {
    case blink::WebMixedContentContextType::kNotMixedContent:
      return Security::MixedContentTypeEnum::None;
    case blink::WebMixedContentContextType::kBlockable:
      return Security::MixedContentTypeEnum::Blockable;
    case blink::WebMixedContentContextType::kOptionallyBlockable:
      return Security::MixedContentTypeEnum::OptionallyBlockable;
    case blink::WebMixedContentContextType::kShouldBeBlockable:
      NOTREACHED();
      return Security::MixedContentTypeEnum::OptionallyBlockable;
    default:
      NOTREACHED();
      return Security::MixedContentTypeEnum::None;
  }
}
