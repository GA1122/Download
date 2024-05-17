static void countEditingEvent(ExecutionContext* executionContext,
                              const Event* event,
                              UseCounter::Feature featureOnInput,
                              UseCounter::Feature featureOnTextArea,
                              UseCounter::Feature featureOnContentEditable,
                              UseCounter::Feature featureOnNonNode) {
  EventTarget* eventTarget = event->target();
  Node* node = eventTarget->toNode();
  if (!node) {
    UseCounter::count(executionContext, featureOnNonNode);
    return;
  }

  if (isHTMLInputElement(node)) {
    UseCounter::count(executionContext, featureOnInput);
    return;
  }

  if (isHTMLTextAreaElement(node)) {
    UseCounter::count(executionContext, featureOnTextArea);
    return;
  }

  TextControlElement* control = enclosingTextControl(node);
  if (isHTMLInputElement(control)) {
    UseCounter::count(executionContext, featureOnInput);
    return;
  }

  if (isHTMLTextAreaElement(control)) {
    UseCounter::count(executionContext, featureOnTextArea);
    return;
  }

  UseCounter::count(executionContext, featureOnContentEditable);
}
