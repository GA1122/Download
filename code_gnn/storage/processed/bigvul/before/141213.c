void Document::SendViolationReport(
    mojom::blink::CSPViolationParamsPtr violation_params) {
  std::unique_ptr<SourceLocation> source_location =
      std::make_unique<SourceLocation>(
          violation_params->source_location->url,
          violation_params->source_location->line_number,
          violation_params->source_location->column_number, nullptr);

  Vector<String> report_endpoints;
  for (const WebString& end_point : violation_params->report_endpoints)
    report_endpoints.push_back(end_point);

  AddConsoleMessage(ConsoleMessage::Create(kSecurityMessageSource,
                                           mojom::ConsoleMessageLevel::kError,
                                           violation_params->console_message));
  GetContentSecurityPolicy()->ReportViolation(
      violation_params->directive,
      ContentSecurityPolicy::GetDirectiveType(
          violation_params->effective_directive),
      violation_params->console_message, KURL(violation_params->blocked_url),
      report_endpoints, violation_params->use_reporting_api,
      violation_params->header,
      static_cast<ContentSecurityPolicyHeaderType>(
          violation_params->disposition),
      ContentSecurityPolicy::ViolationType::kURLViolation,
      std::move(source_location), nullptr  ,
      violation_params->after_redirect ? RedirectStatus::kFollowedRedirect
                                       : RedirectStatus::kNoRedirect,
      nullptr  );
}
