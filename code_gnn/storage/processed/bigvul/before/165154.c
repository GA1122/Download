void HTMLFormElement::ParseAttribute(
    const AttributeModificationParams& params) {
  const QualifiedName& name = params.name;
  if (name == actionAttr) {
    attributes_.ParseAction(params.new_value);
    LogUpdateAttributeIfIsolatedWorldAndInDocument("form", params);

    if (GetDocument().GetInsecureRequestPolicy() & kUpgradeInsecureRequests)
      return;
    KURL action_url = GetDocument().CompleteURL(
        attributes_.Action().IsEmpty() ? GetDocument().Url().GetString()
                                       : attributes_.Action());
    if (MixedContentChecker::IsMixedFormAction(GetDocument().GetFrame(),
                                               action_url)) {
      UseCounter::Count(GetDocument().GetFrame(),
                        WebFeature::kMixedContentFormPresent);
    }
  } else if (name == targetAttr) {
    attributes_.SetTarget(params.new_value);
  } else if (name == methodAttr) {
    attributes_.UpdateMethodType(params.new_value);
  } else if (name == enctypeAttr) {
    attributes_.UpdateEncodingType(params.new_value);
  } else if (name == accept_charsetAttr) {
    attributes_.SetAcceptCharset(params.new_value);
  } else {
    HTMLElement::ParseAttribute(params);
  }
}
