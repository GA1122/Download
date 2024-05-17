void Document::ReportFeaturePolicyViolation(
    mojom::FeaturePolicyFeature feature,
    mojom::FeaturePolicyDisposition disposition,
    const String& message) const {
  if (!origin_trials::FeaturePolicyReportingEnabled(this))
    return;
  LocalFrame* frame = GetFrame();
  if (!frame)
    return;
  const String& feature_name = GetNameForFeature(feature);
  FeaturePolicyViolationReportBody* body =
      MakeGarbageCollected<FeaturePolicyViolationReportBody>(
          feature_name, "Feature policy violation",
          (disposition == mojom::FeaturePolicyDisposition::kReport ? "report"
                                                                   : "enforce"),
          SourceLocation::Capture());
  Report* report = MakeGarbageCollected<Report>("feature-policy-violation",
                                                Url().GetString(), body);
  ReportingContext::From(this)->QueueReport(report);

  bool is_null;
  int line_number = body->lineNumber(is_null);
  line_number = is_null ? 0 : line_number;
  int column_number = body->columnNumber(is_null);
  column_number = is_null ? 0 : column_number;

  frame->GetReportingService()->QueueFeaturePolicyViolationReport(
      Url(), feature_name,
      (disposition == mojom::FeaturePolicyDisposition::kReport ? "report"
                                                               : "enforce"),
      "Feature policy violation", body->sourceFile(), line_number,
      column_number);
  if (disposition == mojom::FeaturePolicyDisposition::kEnforce) {
    frame->Console().AddMessage(ConsoleMessage::Create(
        kViolationMessageSource, mojom::ConsoleMessageLevel::kError,
        (message.IsEmpty() ? ("Feature policy violation: " + feature_name +
                              " is not allowed in this document.")
                           : message)));
  }
}
