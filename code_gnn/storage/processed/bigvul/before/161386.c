void AddExplanations(
    const std::string& security_style,
    const std::vector<SecurityStyleExplanation>& explanations_to_add,
    Explanations* explanations) {
  for (const auto& it : explanations_to_add) {
    std::unique_ptr<protocol::Array<String>> certificate =
        protocol::Array<String>::create();
    if (it.certificate) {
      std::string encoded;
      base::Base64Encode(net::x509_util::CryptoBufferAsStringPiece(
                             it.certificate->cert_buffer()),
                         &encoded);
      certificate->addItem(encoded);

      for (const auto& cert : it.certificate->intermediate_buffers()) {
        base::Base64Encode(
            net::x509_util::CryptoBufferAsStringPiece(cert.get()), &encoded);
        certificate->addItem(encoded);
      }
    }

    explanations->addItem(
        Security::SecurityStateExplanation::Create()
            .SetSecurityState(security_style)
            .SetTitle(it.title)
            .SetSummary(it.summary)
            .SetDescription(it.description)
            .SetCertificate(std::move(certificate))
            .SetMixedContentType(MixedContentTypeToProtocolMixedContentType(
                it.mixed_content_type))
            .Build());
  }
}
