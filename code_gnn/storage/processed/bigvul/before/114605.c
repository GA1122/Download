void RenderThreadImpl::RegisterSchemes() {
  WebString swappedout_scheme(ASCIIToUTF16(chrome::kSwappedOutScheme));
  WebSecurityPolicy::registerURLSchemeAsDisplayIsolated(swappedout_scheme);
  WebSecurityPolicy::registerURLSchemeAsEmptyDocument(swappedout_scheme);
}
