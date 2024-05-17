ContentSecurityPolicy::DirectiveType ContentSecurityPolicy::GetDirectiveType(
    const String& name) {
  if (name == "base-uri")
    return DirectiveType::kBaseURI;
  if (name == "block-all-mixed-content")
    return DirectiveType::kBlockAllMixedContent;
  if (name == "child-src")
    return DirectiveType::kChildSrc;
  if (name == "connect-src")
    return DirectiveType::kConnectSrc;
  if (name == "default-src")
    return DirectiveType::kDefaultSrc;
  if (name == "frame-ancestors")
    return DirectiveType::kFrameAncestors;
  if (name == "frame-src")
    return DirectiveType::kFrameSrc;
  if (name == "font-src")
    return DirectiveType::kFontSrc;
  if (name == "form-action")
    return DirectiveType::kFormAction;
  if (name == "img-src")
    return DirectiveType::kImgSrc;
  if (name == "manifest-src")
    return DirectiveType::kManifestSrc;
  if (name == "media-src")
    return DirectiveType::kMediaSrc;
  if (name == "object-src")
    return DirectiveType::kObjectSrc;
  if (name == "plugin-types")
    return DirectiveType::kPluginTypes;
  if (name == "prefetch-src")
    return DirectiveType::kPrefetchSrc;
  if (name == "report-uri")
    return DirectiveType::kReportURI;
  if (name == "require-sri-for")
    return DirectiveType::kRequireSRIFor;
  if (name == "trusted-types")
    return DirectiveType::kTrustedTypes;
  if (name == "sandbox")
    return DirectiveType::kSandbox;
  if (name == "script-src")
    return DirectiveType::kScriptSrc;
  if (name == "script-src-attr")
    return DirectiveType::kScriptSrcAttr;
  if (name == "script-src-elem")
    return DirectiveType::kScriptSrcElem;
  if (name == "style-src")
    return DirectiveType::kStyleSrc;
  if (name == "style-src-attr")
    return DirectiveType::kStyleSrcAttr;
  if (name == "style-src-elem")
    return DirectiveType::kStyleSrcElem;
  if (name == "treat-as-public-address")
    return DirectiveType::kTreatAsPublicAddress;
  if (name == "upgrade-insecure-requests")
    return DirectiveType::kUpgradeInsecureRequests;
  if (name == "worker-src")
    return DirectiveType::kWorkerSrc;
  if (name == "report-to")
    return DirectiveType::kReportTo;
  if (name == "navigate-to")
    return DirectiveType::kNavigateTo;

  return DirectiveType::kUndefined;
}
