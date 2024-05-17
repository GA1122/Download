void RecordSriResourceIntegrityMismatchEvent(
    SriResourceIntegrityMismatchEvent event) {
  DEFINE_THREAD_SAFE_STATIC_LOCAL(
      EnumerationHistogram, integrity_histogram,
      new EnumerationHistogram("sri.resource_integrity_mismatch_event",
                               kSriResourceIntegrityMismatchEventCount));
  integrity_histogram.Count(event);
}
