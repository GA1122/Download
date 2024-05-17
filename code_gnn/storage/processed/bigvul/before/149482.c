bool ContentSecurityPolicy::shouldBypassMainWorld(
    const ExecutionContext* context) {
  if (context && context->isDocument()) {
    const Document* document = toDocument(context);
    if (document->frame())
      return document->frame()->script().shouldBypassMainWorldCSP();
  }
  return false;
}
