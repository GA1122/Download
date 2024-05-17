void Document::DidEnforceInsecureNavigationsSet() {
  if (!GetFrame())
    return;
  GetFrame()->Client()->DidEnforceInsecureNavigationsSet(
      SecurityContext::SerializeInsecureNavigationSet(
          *InsecureNavigationsToUpgrade()));
}
