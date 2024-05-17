void SecurityHandler::DidChangeVisibleSecurityState() {
  DCHECK(enabled_);

  SecurityStyleExplanations security_style_explanations;
  blink::WebSecurityStyle security_style =
      web_contents()->GetDelegate()->GetSecurityStyle(
          web_contents(), &security_style_explanations);

  const std::string security_state =
      SecurityStyleToProtocolSecurityState(security_style);

  std::unique_ptr<Explanations> explanations = Explanations::create();
  AddExplanations(Security::SecurityStateEnum::Insecure,
                  security_style_explanations.insecure_explanations,
                  explanations.get());
  AddExplanations(Security::SecurityStateEnum::Neutral,
                  security_style_explanations.neutral_explanations,
                  explanations.get());
  AddExplanations(Security::SecurityStateEnum::Secure,
                  security_style_explanations.secure_explanations,
                  explanations.get());
  AddExplanations(Security::SecurityStateEnum::Info,
                  security_style_explanations.info_explanations,
                  explanations.get());

  std::unique_ptr<Security::InsecureContentStatus> insecure_status =
      Security::InsecureContentStatus::Create()
          .SetRanMixedContent(security_style_explanations.ran_mixed_content)
          .SetDisplayedMixedContent(
              security_style_explanations.displayed_mixed_content)
          .SetContainedMixedForm(
              security_style_explanations.contained_mixed_form)
          .SetRanContentWithCertErrors(
              security_style_explanations.ran_content_with_cert_errors)
          .SetDisplayedContentWithCertErrors(
              security_style_explanations.displayed_content_with_cert_errors)
          .SetRanInsecureContentStyle(SecurityStyleToProtocolSecurityState(
              security_style_explanations.ran_insecure_content_style))
          .SetDisplayedInsecureContentStyle(
              SecurityStyleToProtocolSecurityState(
                  security_style_explanations.displayed_insecure_content_style))
          .Build();

  frontend_->SecurityStateChanged(
      security_state,
      security_style_explanations.scheme_is_cryptographic,
      std::move(explanations),
      std::move(insecure_status),
      Maybe<std::string>(security_style_explanations.summary));
}
