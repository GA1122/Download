HTMLIFrameElement::ConstructContainerPolicy(Vector<String>* messages,
                                            bool* old_syntax) const {
  scoped_refptr<SecurityOrigin> src_origin = GetOriginForFeaturePolicy();
  scoped_refptr<SecurityOrigin> self_origin = GetDocument().GetSecurityOrigin();
  Vector<WebParsedFeaturePolicyDeclaration> container_policy =
      ParseFeaturePolicyAttribute(allow_, self_origin, src_origin, messages,
                                  old_syntax);

  if (AllowFullscreen()) {
    bool has_fullscreen_policy = false;
    for (const auto& declaration : container_policy) {
      if (declaration.feature == WebFeaturePolicyFeature::kFullscreen) {
        has_fullscreen_policy = true;
        if (messages) {
          messages->push_back(
              "allow attribute is overriding 'allowfullscreen'.");
        }
        break;
      }
    }
    if (!has_fullscreen_policy) {
      WebParsedFeaturePolicyDeclaration whitelist;
      whitelist.feature = WebFeaturePolicyFeature::kFullscreen;
      whitelist.matches_all_origins = true;
      whitelist.origins = Vector<WebSecurityOrigin>(0UL);
      container_policy.push_back(whitelist);
    }
  }
  if (AllowPaymentRequest()) {
    bool has_payment_policy = false;
    for (const auto& declaration : container_policy) {
      if (declaration.feature == WebFeaturePolicyFeature::kPayment) {
        has_payment_policy = true;
        if (messages) {
          messages->push_back(
              "allow attribute is overriding 'allowpaymentrequest'.");
        }
        break;
      }
    }
    if (!has_payment_policy) {
      WebParsedFeaturePolicyDeclaration whitelist;
      whitelist.feature = WebFeaturePolicyFeature::kPayment;
      whitelist.matches_all_origins = true;
      whitelist.origins = Vector<WebSecurityOrigin>(0UL);
      container_policy.push_back(whitelist);
    }
  }

  return container_policy;
}
