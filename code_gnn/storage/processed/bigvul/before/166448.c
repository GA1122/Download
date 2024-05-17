void RecordParallelDownloadCreationEvent(ParallelDownloadCreationEvent event) {
  UMA_HISTOGRAM_ENUMERATION("Download.ParallelDownload.CreationEvent", event,
                            ParallelDownloadCreationEvent::COUNT);
}
