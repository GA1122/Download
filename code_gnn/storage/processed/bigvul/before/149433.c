void ContentSecurityPolicy::dispatchViolationEvents(
    const SecurityPolicyViolationEventInit& violationData,
    Element* element) {
  EventQueue* queue = m_executionContext->getEventQueue();
  if (!queue)
    return;

  SecurityPolicyViolationEvent* event = SecurityPolicyViolationEvent::create(
      EventTypeNames::securitypolicyviolation, violationData);
  DCHECK(event->bubbles());

  if (m_executionContext->isDocument()) {
    Document* document = toDocument(m_executionContext);
    if (element && element->isConnected() && element->document() == document)
      event->setTarget(element);
    else
      event->setTarget(document);
  } else if (m_executionContext->isWorkerGlobalScope()) {
    event->setTarget(toWorkerGlobalScope(m_executionContext));
  }
  queue->enqueueEvent(event);
}
