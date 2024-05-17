void HTMLIFrameElement::ParseAttribute(
    const AttributeModificationParams& params) {
  const QualifiedName& name = params.name;
  const AtomicString& value = params.new_value;
  if (name == nameAttr) {
    if (IsInDocumentTree() && GetDocument().IsHTMLDocument()) {
      HTMLDocument& document = ToHTMLDocument(this->GetDocument());
      document.RemoveNamedItem(name_);
      document.AddNamedItem(value);
    }
    AtomicString old_name = name_;
    name_ = value;
    if (name_ != old_name)
      FrameOwnerPropertiesChanged();
  } else if (name == sandboxAttr) {
    sandbox_->DidUpdateAttributeValue(params.old_value, value);
    String invalid_tokens;
    SetSandboxFlags(value.IsNull() ? kSandboxNone
                                   : ParseSandboxPolicy(sandbox_->TokenSet(),
                                                        invalid_tokens));
    if (!invalid_tokens.IsNull()) {
      GetDocument().AddConsoleMessage(ConsoleMessage::Create(
          kOtherMessageSource, kErrorMessageLevel,
          "Error while parsing the 'sandbox' attribute: " + invalid_tokens));
    }
    UseCounter::Count(GetDocument(), WebFeature::kSandboxViaIFrame);
  } else if (name == referrerpolicyAttr) {
    referrer_policy_ = kReferrerPolicyDefault;
    if (!value.IsNull()) {
      SecurityPolicy::ReferrerPolicyFromString(
          value, kSupportReferrerPolicyLegacyKeywords, &referrer_policy_);
      UseCounter::Count(GetDocument(),
                        WebFeature::kHTMLIFrameElementReferrerPolicyAttribute);
    }
  } else if (name == allowfullscreenAttr) {
    bool old_allow_fullscreen = allow_fullscreen_;
    allow_fullscreen_ = !value.IsNull();
    if (allow_fullscreen_ != old_allow_fullscreen) {
      if (allow_fullscreen_ && ContentFrame()) {
        UseCounter::Count(
            GetDocument(),
            WebFeature::
                kHTMLIFrameElementAllowfullscreenAttributeSetAfterContentLoad);
      }
      FrameOwnerPropertiesChanged();
      UpdateContainerPolicy();
    }
  } else if (name == allowpaymentrequestAttr) {
    bool old_allow_payment_request = allow_payment_request_;
    allow_payment_request_ = !value.IsNull();
    if (allow_payment_request_ != old_allow_payment_request) {
      FrameOwnerPropertiesChanged();
      UpdateContainerPolicy();
    }
  } else if (RuntimeEnabledFeatures::EmbedderCSPEnforcementEnabled() &&
             name == cspAttr) {
    if (!ContentSecurityPolicy::IsValidCSPAttr(value.GetString())) {
      csp_ = g_null_atom;
      GetDocument().AddConsoleMessage(ConsoleMessage::Create(
          kOtherMessageSource, kErrorMessageLevel,
          "'csp' attribute is not a valid policy: " + value));
      return;
    }
    if (csp_ != value) {
      csp_ = value;
      FrameOwnerPropertiesChanged();
    }
  } else if (RuntimeEnabledFeatures::FeaturePolicyEnabled() &&
             name == allowAttr) {
    if (allow_ != value) {
      allow_ = value;
      Vector<String> messages;
      bool old_syntax = false;
      UpdateContainerPolicy(&messages, &old_syntax);
      if (!messages.IsEmpty()) {
        for (const String& message : messages) {
          GetDocument().AddConsoleMessage(ConsoleMessage::Create(
              kOtherMessageSource, kWarningMessageLevel, message));
        }
      }
      if (!value.IsEmpty()) {
        if (old_syntax) {
          UseCounter::Count(
              GetDocument(),
              WebFeature::kFeaturePolicyAllowAttributeDeprecatedSyntax);
        } else {
          UseCounter::Count(GetDocument(),
                            WebFeature::kFeaturePolicyAllowAttribute);
        }
      }
    }
  } else {
    if (name == srcAttr)
      LogUpdateAttributeIfIsolatedWorldAndInDocument("iframe", params);
    HTMLFrameElementBase::ParseAttribute(params);
  }
}
