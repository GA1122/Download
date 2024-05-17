static bool ExecuteInsertElement(LocalFrame& frame, HTMLElement* content) {
  DCHECK(frame.GetDocument());
  DocumentFragment* fragment = DocumentFragment::Create(*frame.GetDocument());
  DummyExceptionStateForTesting exception_state;
  fragment->AppendChild(content, exception_state);
  if (exception_state.HadException())
    return false;
  return ExecuteInsertFragment(frame, fragment);
}
