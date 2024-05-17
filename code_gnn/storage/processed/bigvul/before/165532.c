void ContentSecurityPolicy::PostViolationReport(
    const SecurityPolicyViolationEventInit* violation_data,
    LocalFrame* context_frame,
    const Vector<String>& report_endpoints,
    bool use_reporting_api) {

  std::unique_ptr<JSONObject> csp_report = JSONObject::Create();
  csp_report->SetString("document-uri", violation_data->documentURI());
  csp_report->SetString("referrer", violation_data->referrer());
  csp_report->SetString("violated-directive",
                        violation_data->violatedDirective());
  csp_report->SetString("effective-directive",
                        violation_data->effectiveDirective());
  csp_report->SetString("original-policy", violation_data->originalPolicy());
  csp_report->SetString("disposition", violation_data->disposition());
  csp_report->SetString("blocked-uri", violation_data->blockedURI());
  if (violation_data->lineNumber())
    csp_report->SetInteger("line-number", violation_data->lineNumber());
  if (violation_data->columnNumber())
    csp_report->SetInteger("column-number", violation_data->columnNumber());
  if (!violation_data->sourceFile().IsEmpty())
    csp_report->SetString("source-file", violation_data->sourceFile());
  csp_report->SetInteger("status-code", violation_data->statusCode());

  csp_report->SetString("script-sample", violation_data->sample());

  std::unique_ptr<JSONObject> report_object = JSONObject::Create();
  report_object->SetObject("csp-report", std::move(csp_report));
  String stringified_report = report_object->ToJSONString();

  if (ShouldSendViolationReport(stringified_report)) {
    DidSendViolationReport(stringified_report);

    Document* document =
        context_frame ? context_frame->GetDocument() : this->GetDocument();
    if (!document)
      return;

    LocalFrame* frame = document->GetFrame();
    if (!frame)
      return;

    scoped_refptr<EncodedFormData> report =
        EncodedFormData::Create(stringified_report.Utf8());

    DEFINE_STATIC_LOCAL(ReportingServiceProxyPtrHolder,
                        reporting_service_proxy_holder, ());

    for (const auto& report_endpoint : report_endpoints) {
      if (use_reporting_api) {
        reporting_service_proxy_holder.QueueCspViolationReport(
            document->Url(), report_endpoint, violation_data);
      } else {
        DCHECK(!context_frame || !execution_context_);
        DCHECK(!context_frame ||
               GetDirectiveType(violation_data->effectiveDirective()) ==
                   DirectiveType::kFrameAncestors);
        KURL url =
            context_frame
                ? frame->GetDocument()->CompleteURLWithOverride(
                      report_endpoint, KURL(violation_data->blockedURI()))
                : frame->GetDocument()->CompleteURLWithOverride(
                      report_endpoint, frame->GetDocument()->FallbackBaseURL());
        PingLoader::SendViolationReport(
            frame, url, report,
            PingLoader::kContentSecurityPolicyViolationReport);
      }
    }
  }
}
