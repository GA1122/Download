void FrameLoader::ModifyRequestForCSP(ResourceRequest& resource_request,
                                      Document* origin_document) const {
  if (!RequiredCSP().IsEmpty()) {
    DCHECK(
        ContentSecurityPolicy::IsValidCSPAttr(RequiredCSP().GetString(), ""));
    resource_request.SetHTTPHeaderField(http_names::kSecRequiredCSP,
                                        RequiredCSP());
  }

  if (resource_request.GetFrameType() !=
      network::mojom::RequestContextFrameType::kNone) {
    if (!resource_request.HttpHeaderField(http_names::kUpgradeInsecureRequests)
             .IsNull()) {
      return;
    }

    resource_request.SetHTTPHeaderField(http_names::kUpgradeInsecureRequests,
                                        "1");
  }

  UpgradeInsecureRequest(resource_request, origin_document);
}
