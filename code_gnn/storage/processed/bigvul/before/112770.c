void ReportPrintDestinationHistogram(enum PrintDestinationBuckets event) {
  UMA_HISTOGRAM_ENUMERATION("PrintPreview.DestinationAction", event,
                            PRINT_DESTINATION_BUCKET_BOUNDARY);
}
