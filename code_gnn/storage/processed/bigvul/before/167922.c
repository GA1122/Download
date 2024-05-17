void LocalFrame::DocumentAttached() {
  DCHECK(GetDocument());
  GetEditor().Clear();
  GetEventHandler().Clear();
  Selection().DocumentAttached(GetDocument());
  GetInputMethodController().DocumentAttached(GetDocument());
  GetSpellChecker().DocumentAttached(GetDocument());
  GetTextSuggestionController().DocumentAttached(GetDocument());
}
