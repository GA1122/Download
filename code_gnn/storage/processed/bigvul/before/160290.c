static void ConfigureRequest(
    FetchParameters& params,
    ImageLoader::BypassMainWorldBehavior bypass_behavior,
    Element& element,
    const ClientHintsPreferences& client_hints_preferences) {
  if (bypass_behavior == ImageLoader::kBypassMainWorldCSP)
    params.SetContentSecurityCheck(kDoNotCheckContentSecurityPolicy);

  CrossOriginAttributeValue cross_origin = GetCrossOriginAttributeValue(
      element.FastGetAttribute(HTMLNames::crossoriginAttr));
  if (cross_origin != kCrossOriginAttributeNotSet) {
    params.SetCrossOriginAccessControl(
        element.GetDocument().GetSecurityOrigin(), cross_origin);
  }

  if (client_hints_preferences.ShouldSend(
          mojom::WebClientHintsType::kResourceWidth) &&
      IsHTMLImageElement(element))
    params.SetResourceWidth(ToHTMLImageElement(element).GetResourceWidth());
}
