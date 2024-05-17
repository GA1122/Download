void FrameLoader::ModifyRequestForCSP(ResourceRequest& resource_request,
                                      Document* origin_document) const {
  if (RuntimeEnabledFeatures::EmbedderCSPEnforcementEnabled() &&
      !RequiredCSP().IsEmpty()) {
    DCHECK(
        ContentSecurityPolicy::IsValidCSPAttr(RequiredCSP().GetString(), ""));
    resource_request.SetHTTPHeaderField(HTTPNames::Sec_Required_CSP,
                                        RequiredCSP());
  }

  if (resource_request.GetFrameType() !=
      network::mojom::RequestContextFrameType::kNone) {
    if (!resource_request.HttpHeaderField(HTTPNames::Upgrade_Insecure_Requests)
             .IsNull()) {
      return;
    }

    resource_request.SetHTTPHeaderField(HTTPNames::Upgrade_Insecure_Requests,
                                        "1");
  }

  UpgradeInsecureRequest(resource_request, origin_document);
}
