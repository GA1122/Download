bool ContentSecurityPolicy::shouldEnforceEmbeddersPolicy(
    const ResourceResponse& response,
    SecurityOrigin* parentOrigin) {
  if (response.url().isEmpty() || response.url().protocolIsAbout() ||
      response.url().protocolIsData() || response.url().protocolIs("blob") ||
      response.url().protocolIs("filesystem")) {
    return true;
  }

  if (parentOrigin->canAccess(SecurityOrigin::create(response.url()).get()))
    return true;

  String header = response.httpHeaderField(HTTPNames::Allow_CSP_From);
  header = header.stripWhiteSpace();
  if (header == "*")
    return true;
  if (RefPtr<SecurityOrigin> childOrigin =
          SecurityOrigin::createFromString(header)) {
    return parentOrigin->canAccess(childOrigin.get());
  }

  return false;
}
