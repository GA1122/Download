bool ContentSecurityPolicy::ShouldEnforceEmbeddersPolicy(
    const ResourceResponse& response,
    const SecurityOrigin* parent_origin) {
  if (response.Url().IsEmpty() || response.Url().ProtocolIsAbout() ||
      response.Url().ProtocolIsData() || response.Url().ProtocolIs("blob") ||
      response.Url().ProtocolIs("filesystem")) {
    return true;
  }

  if (parent_origin->CanAccess(SecurityOrigin::Create(response.Url()).get()))
    return true;

  String header = response.HttpHeaderField(http_names::kAllowCSPFrom);
  header = header.StripWhiteSpace();
  if (header == "*")
    return true;
  if (scoped_refptr<const SecurityOrigin> child_origin =
          SecurityOrigin::CreateFromString(header)) {
    return parent_origin->CanAccess(child_origin.get());
  }

  return false;
}
