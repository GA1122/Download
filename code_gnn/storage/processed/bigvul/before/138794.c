bool RenderFrameHostImpl::SchemeShouldBypassCSP(
    const base::StringPiece& scheme) {
  const auto& bypassing_schemes = url::GetCSPBypassingSchemes();
  return std::find(bypassing_schemes.begin(), bypassing_schemes.end(),
                   scheme) != bypassing_schemes.end();
}
