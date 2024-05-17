ScriptPromise LocalDOMWindow::getComputedAccessibleNode(
    ScriptState* script_state,
    Element* element) {
  DCHECK(element);
  element->GetDocument().GetPage()->GetSettings().SetAccessibilityEnabled(true);
  ComputedAccessibleNodePromiseResolver* resolver =
      ComputedAccessibleNodePromiseResolver::Create(script_state, *element);
  ScriptPromise promise = resolver->Promise();
  resolver->ComputeAccessibleNode();
  return promise;
}
