void Document::DidEnforceInsecureRequestPolicy() {
  if (!GetFrame())
    return;
  GetFrame()->Client()->DidEnforceInsecureRequestPolicy(
      GetInsecureRequestPolicy());
}
