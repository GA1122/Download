const QualifiedName& trackIndexAttrName() {
  DEFINE_STATIC_LOCAL(QualifiedName, trackIndexAttr,
                      (nullAtom, "data-track-index", nullAtom));
  return trackIndexAttr;
}
