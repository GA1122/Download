static WeakDocumentSet& liveDocumentSet() {
  DEFINE_STATIC_LOCAL(blink::Persistent<WeakDocumentSet>, set,
                      (new WeakDocumentSet));
  return *set;
}
