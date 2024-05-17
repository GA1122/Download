const char* ContentSecurityPolicy::getDirectiveName(const DirectiveType& type) {
  switch (type) {
    case DirectiveType::BaseURI:
      return "base-uri";
    case DirectiveType::BlockAllMixedContent:
      return "block-all-mixed-content";
    case DirectiveType::ChildSrc:
      return "child-src";
    case DirectiveType::ConnectSrc:
      return "connect-src";
    case DirectiveType::DefaultSrc:
      return "default-src";
    case DirectiveType::FrameAncestors:
      return "frame-ancestors";
    case DirectiveType::FrameSrc:
      return "frame-src";
    case DirectiveType::FontSrc:
      return "font-src";
    case DirectiveType::FormAction:
      return "form-action";
    case DirectiveType::ImgSrc:
      return "img-src";
    case DirectiveType::ManifestSrc:
      return "manifest-src";
    case DirectiveType::MediaSrc:
      return "media-src";
    case DirectiveType::ObjectSrc:
      return "object-src";
    case DirectiveType::PluginTypes:
      return "plugin-types";
    case DirectiveType::ReportURI:
      return "report-uri";
    case DirectiveType::RequireSRIFor:
      return "require-sri-for";
    case DirectiveType::Sandbox:
      return "sandbox";
    case DirectiveType::ScriptSrc:
      return "script-src";
    case DirectiveType::StyleSrc:
      return "style-src";
    case DirectiveType::TreatAsPublicAddress:
      return "treat-as-public-address";
    case DirectiveType::UpgradeInsecureRequests:
      return "upgrade-insecure-requests";
    case DirectiveType::WorkerSrc:
      return "worker-src";
    case DirectiveType::Undefined:
      NOTREACHED();
      return "";
  }

  NOTREACHED();
  return "";
}
