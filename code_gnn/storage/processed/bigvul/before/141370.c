void showLiveDocumentInstances() {
  WeakDocumentSet& set = liveDocumentSet();
  fprintf(stderr, "There are %u documents currently alive:\n", set.size());
  for (blink::Document* document : set)
    fprintf(stderr, "- Document %p URL: %s\n", document,
            document->Url().GetString().Utf8().data());
}
