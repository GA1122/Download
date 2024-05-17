void ContentSecurityPolicy::reportViolation(
    const String& directiveText,
    const DirectiveType& effectiveType,
    const String& consoleMessage,
    const KURL& blockedURL,
    const Vector<String>& reportEndpoints,
    const String& header,
    ContentSecurityPolicyHeaderType headerType,
    ViolationType violationType,
    LocalFrame* contextFrame,
    RedirectStatus redirectStatus,
    int contextLine,
    Element* element,
    const String& source) {
  ASSERT(violationType == URLViolation || blockedURL.isEmpty());

  if (!m_executionContext && !contextFrame) {
    DCHECK(effectiveType == DirectiveType::ChildSrc ||
           effectiveType == DirectiveType::FrameSrc ||
           effectiveType == DirectiveType::PluginTypes);
    return;
  }

  DCHECK((m_executionContext && !contextFrame) ||
         ((effectiveType == DirectiveType::FrameAncestors) && contextFrame));

  SecurityPolicyViolationEventInit violationData;

  ExecutionContext* relevantContext =
      contextFrame ? contextFrame->document() : m_executionContext;
  DCHECK(relevantContext);
  gatherSecurityPolicyViolationEventData(
      violationData, relevantContext, directiveText, effectiveType, blockedURL,
      header, redirectStatus, headerType, violationType, contextLine, source);

  if (!violationData.sourceFile().isEmpty() &&
      shouldBypassContentSecurityPolicy(
          KURL(ParsedURLString, violationData.sourceFile()))) {
    return;
  }

  postViolationReport(violationData, contextFrame, reportEndpoints);

  if (m_executionContext) {
    TaskRunnerHelper::get(TaskType::Networking, m_executionContext)
        ->postTask(BLINK_FROM_HERE,
                   WTF::bind(&ContentSecurityPolicy::dispatchViolationEvents,
                             wrapPersistent(this), violationData,
                             wrapPersistent(element)));
  }
}
