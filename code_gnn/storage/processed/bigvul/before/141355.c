Element* Document::scrollingElement() {
  if (RuntimeEnabledFeatures::ScrollTopLeftInteropEnabled() && InQuirksMode())
    UpdateStyleAndLayoutTree();
  return ScrollingElementNoLayout();
}
