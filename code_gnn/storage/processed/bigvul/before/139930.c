static void assertShadowRootChildren(ShadowRoot& shadowRoot) {
#if DCHECK_IS_ON()
  unsigned numberOfChildren = shadowRoot.countChildren();
  DCHECK_LE(numberOfChildren, 2u);
  Node* firstChild = shadowRoot.firstChild();
  Node* lastChild = shadowRoot.lastChild();
  if (numberOfChildren == 1) {
    DCHECK(firstChild->isTextTrackContainer() || firstChild->isMediaControls());
  } else if (numberOfChildren == 2) {
    DCHECK(firstChild->isTextTrackContainer());
    DCHECK(lastChild->isMediaControls());
  }
#endif
}
