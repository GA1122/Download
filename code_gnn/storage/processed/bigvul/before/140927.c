PresentationConnection* PresentationConnection::take(
    ScriptPromiseResolver* resolver,
    const WebPresentationSessionInfo& sessionInfo,
    PresentationRequest* request) {
  ASSERT(resolver);
  ASSERT(request);
  ASSERT(resolver->getExecutionContext()->isDocument());

  Document* document = toDocument(resolver->getExecutionContext());
  if (!document->frame())
    return nullptr;

  PresentationController* controller =
      PresentationController::from(*document->frame());
  if (!controller)
    return nullptr;

  return take(controller, sessionInfo, request);
}
