static void RecordLoadReasonToHistogram(WouldLoadReason reason) {
  DEFINE_STATIC_LOCAL(EnumerationHistogram, unseen_frame_histogram,
                      ("Navigation.DeferredDocumentLoading.StatesV4",
                       static_cast<int>(WouldLoadReason::kCount)));
  unseen_frame_histogram.Count(static_cast<int>(reason));
}
