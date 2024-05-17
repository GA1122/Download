static void GatherSecurityPolicyViolationEventData(
    SecurityPolicyViolationEventInit* init,
    ExecutionContext* context,
    const String& directive_text,
    const ContentSecurityPolicy::DirectiveType& effective_type,
    const KURL& blocked_url,
    const String& header,
    RedirectStatus redirect_status,
    ContentSecurityPolicyHeaderType header_type,
    ContentSecurityPolicy::ViolationType violation_type,
    std::unique_ptr<SourceLocation> source_location,
    const String& script_source) {
  if (effective_type == ContentSecurityPolicy::DirectiveType::kFrameAncestors) {
    String stripped_url = StripURLForUseInReport(
        context, blocked_url, RedirectStatus::kNoRedirect,
        ContentSecurityPolicy::DirectiveType::kDefaultSrc);
    init->setDocumentURI(stripped_url);
    init->setBlockedURI(stripped_url);
  } else {
    String stripped_url = StripURLForUseInReport(
        context, context->Url(), RedirectStatus::kNoRedirect,
        ContentSecurityPolicy::DirectiveType::kDefaultSrc);
    init->setDocumentURI(stripped_url);
    switch (violation_type) {
      case ContentSecurityPolicy::kInlineViolation:
        init->setBlockedURI("inline");
        break;
      case ContentSecurityPolicy::kEvalViolation:
        init->setBlockedURI("eval");
        break;
      case ContentSecurityPolicy::kURLViolation:
        init->setBlockedURI(StripURLForUseInReport(
            context, blocked_url, redirect_status, effective_type));
        break;
    }
  }

  String effective_directive =
      ContentSecurityPolicy::GetDirectiveName(effective_type);
  init->setViolatedDirective(effective_directive);
  init->setEffectiveDirective(effective_directive);
  init->setOriginalPolicy(header);
  init->setDisposition(header_type == kContentSecurityPolicyHeaderTypeEnforce
                           ? "enforce"
                           : "report");
  init->setStatusCode(0);

  if (auto* document = DynamicTo<Document>(*context)) {
    init->setReferrer(document->referrer());
    if (!SecurityOrigin::IsSecure(context->Url()) && document->Loader())
      init->setStatusCode(document->Loader()->GetResponse().HttpStatusCode());
  }

  if (!source_location)
    source_location = SourceLocation::Capture(context);
  if (source_location->LineNumber()) {
    KURL source = KURL(source_location->Url());
    init->setSourceFile(StripURLForUseInReport(context, source, redirect_status,
                                               effective_type));
    init->setLineNumber(source_location->LineNumber());
    init->setColumnNumber(source_location->ColumnNumber());
  } else {
    init->setSourceFile(String());
    init->setLineNumber(0);
    init->setColumnNumber(0);
  }

  if (!script_source.IsEmpty()) {
    init->setSample(script_source.StripWhiteSpace().Left(
        ContentSecurityPolicy::kMaxSampleLength));
  }
}
