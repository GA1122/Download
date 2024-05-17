void RecordPlayPromiseRejected(PlayPromiseRejectReason reason) {
  DEFINE_STATIC_LOCAL(EnumerationHistogram, histogram,
                      ("Media.MediaElement.PlayPromiseReject",
                       static_cast<int>(PlayPromiseRejectReason::kCount)));
  histogram.Count(static_cast<int>(reason));
}
