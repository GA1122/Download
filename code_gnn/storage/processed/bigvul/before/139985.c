TextTrackContainer& HTMLMediaElement::ensureTextTrackContainer() {
  ShadowRoot& shadowRoot = ensureUserAgentShadowRoot();
  assertShadowRootChildren(shadowRoot);

  Node* firstChild = shadowRoot.firstChild();
  if (firstChild && firstChild->isTextTrackContainer())
    return toTextTrackContainer(*firstChild);

  TextTrackContainer* textTrackContainer =
      TextTrackContainer::create(document());

  shadowRoot.insertBefore(textTrackContainer, firstChild);

  assertShadowRootChildren(shadowRoot);

  return *textTrackContainer;
}
