const char* ContentSecurityPolicy::GetDirectiveName(const DirectiveType& type) {
  switch (type) {
    case DirectiveType::kBaseURI:
      return "base-uri";
    case DirectiveType::kBlockAllMixedContent:
      return "block-all-mixed-content";
    case DirectiveType::kChildSrc:
      return "child-src";
    case DirectiveType::kConnectSrc:
      return "connect-src";
    case DirectiveType::kDefaultSrc:
      return "default-src";
    case DirectiveType::kFrameAncestors:
      return "frame-ancestors";
    case DirectiveType::kFrameSrc:
      return "frame-src";
    case DirectiveType::kFontSrc:
      return "font-src";
    case DirectiveType::kFormAction:
      return "form-action";
    case DirectiveType::kImgSrc:
      return "img-src";
    case DirectiveType::kManifestSrc:
      return "manifest-src";
    case DirectiveType::kMediaSrc:
      return "media-src";
    case DirectiveType::kObjectSrc:
      return "object-src";
    case DirectiveType::kPrefetchSrc:
      return "prefetch-src";
    case DirectiveType::kPluginTypes:
      return "plugin-types";
    case DirectiveType::kReportURI:
      return "report-uri";
    case DirectiveType::kRequireSRIFor:
      return "require-sri-for";
    case DirectiveType::kTrustedTypes:
      return "trusted-types";
    case DirectiveType::kSandbox:
      return "sandbox";
    case DirectiveType::kScriptSrc:
      return "script-src";
    case DirectiveType::kScriptSrcAttr:
      return "script-src-attr";
    case DirectiveType::kScriptSrcElem:
      return "script-src-elem";
    case DirectiveType::kStyleSrc:
      return "style-src";
    case DirectiveType::kStyleSrcAttr:
      return "style-src-attr";
    case DirectiveType::kStyleSrcElem:
      return "style-src-elem";
    case DirectiveType::kTreatAsPublicAddress:
      return "treat-as-public-address";
    case DirectiveType::kUpgradeInsecureRequests:
      return "upgrade-insecure-requests";
    case DirectiveType::kWorkerSrc:
      return "worker-src";
    case DirectiveType::kReportTo:
      return "report-to";
    case DirectiveType::kNavigateTo:
      return "navigate-to";
    case DirectiveType::kUndefined:
      NOTREACHED();
      return "";
  }

  NOTREACHED();
  return "";
}
