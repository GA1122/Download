void PrintPreviewHandler::HandleReportDestinationEvent(const ListValue* args) {
  int event_number;
  bool ret = args->GetInteger(0, &event_number);
  if (!ret)
    return;
  enum PrintDestinationBuckets event =
      static_cast<enum PrintDestinationBuckets>(event_number);
  if (event >= PRINT_DESTINATION_BUCKET_BOUNDARY)
    return;
  ReportPrintDestinationHistogram(event);
}
