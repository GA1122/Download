void Document::EnforceInsecureRequestPolicy(WebInsecureRequestPolicy policy) {
  SetInsecureRequestPolicy(GetInsecureRequestPolicy() | policy);
  if (GetFrame())
    GetFrame()->Client()->DidEnforceInsecureRequestPolicy(
        GetInsecureRequestPolicy());
}
