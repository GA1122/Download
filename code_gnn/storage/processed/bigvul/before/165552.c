void ContentSecurityPolicy::ReportViolation(
    const String& directive_text,
    const DirectiveType& effective_type,
    const String& console_message,
    const KURL& blocked_url,
    const Vector<String>& report_endpoints,
    bool use_reporting_api,
    const String& header,
    ContentSecurityPolicyHeaderType header_type,
    ViolationType violation_type,
    std::unique_ptr<SourceLocation> source_location,
    LocalFrame* context_frame,
    RedirectStatus redirect_status,
    Element* element,
    const String& source) {
  DCHECK(violation_type == kURLViolation || blocked_url.IsEmpty());

  if (!execution_context_ && !context_frame) {
    DCHECK(effective_type == DirectiveType::kChildSrc ||
           effective_type == DirectiveType::kFrameSrc ||
           effective_type == DirectiveType::kPluginTypes);
    return;
  }

  DCHECK((execution_context_ && !context_frame) ||
         ((effective_type == DirectiveType::kFrameAncestors) && context_frame));

  SecurityPolicyViolationEventInit* violation_data =
      SecurityPolicyViolationEventInit::Create();

  ExecutionContext* relevant_context =
      context_frame ? context_frame->GetDocument() : execution_context_;
  DCHECK(relevant_context);
  GatherSecurityPolicyViolationEventData(
      violation_data, relevant_context, directive_text, effective_type,
      blocked_url, header, redirect_status, header_type, violation_type,
      std::move(source_location), source);

  if (!violation_data->sourceFile().IsEmpty() &&
      ShouldBypassContentSecurityPolicy(KURL(violation_data->sourceFile()),
                                        execution_context_)) {
    return;
  }

  PostViolationReport(violation_data, context_frame, report_endpoints,
                      use_reporting_api);

  if (execution_context_) {
    execution_context_->GetTaskRunner(TaskType::kNetworking)
        ->PostTask(
            FROM_HERE,
            WTF::Bind(&ContentSecurityPolicy::DispatchViolationEvents,
                      WrapPersistent(this), WrapPersistent(violation_data),
                      WrapPersistent(element)));
  }
}
