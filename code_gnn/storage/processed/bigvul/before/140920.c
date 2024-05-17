WebPresentationClient* presentationClient(ExecutionContext* executionContext) {
  ASSERT(executionContext && executionContext->isDocument());

  Document* document = toDocument(executionContext);
  if (!document->frame())
    return nullptr;
  PresentationController* controller =
      PresentationController::from(*document->frame());
  return controller ? controller->client() : nullptr;
}
